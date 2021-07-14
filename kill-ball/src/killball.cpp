#include "leep.h"
#include "kill-ball.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <ImGui/imgui.h>

using namespace killball;

char g_nickname[16];
char g_ip_server[16];
leep::Connection g_connection;

const char *PlayerStateStr(StateCode s)
{
    switch (s)
    {
        case S_PLAYER_NONE:
            return "...";
        case S_PLAYER_CONNECTED:
            return "Connected";
        case S_PLAYER_READY:
            return "Ready";
        case S_PLAYER_PLAYING:
            return "Playing";
        case S_PLAYER_DEAD:
            return "Dead";
        case S_PLAYER_WINNER:
            return "Winner";
        case S_PLAYER_DISCONNECTED:
            return "Disconnected";
        default:
            return "?";
    }
}

void ConnectWindow()
{
    static bool show = true;
    if (!ImGui::Begin("Connection Window", &show, ImGuiWindowFlags_AlwaysAutoResize))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }
    switch(GetGame().state_.match_state_)
    {
        case S_MATCH_WAITING_FOR_PLAYERS:
            ImGui::Text("Match state: Waiting for players.");
            break;
        
        case S_MATCH_PLAYING:
            ImGui::Text("Match state: Playing.");
            break;
        
        case S_MATCH_FINISHED:
            ImGui::Text("Match state: Finished.");
            break;
        
        default:
            ImGui::Text("Match state: Unknown.");
            break;
    }

    static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
    ImVec2 size = ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 5);
    if (ImGui::BeginTable("##table1", 3, flags, size))
    {
        ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
        ImGui::TableSetupColumn("CHARACTER", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("NICKNAME", ImGuiTableColumnFlags_None);
        ImGui::TableSetupColumn("STATE", ImGuiTableColumnFlags_None);
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Golde");
        ImGui::TableSetColumnIndex(1);
        if (GetGame().state_.players_[0][0] != '\0')
            ImGui::Text("%s", GetGame().state_.players_[0]);
        else
            ImGui::Text("...");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%s", PlayerStateStr(GetGame().state_.player_states_[0]));

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Peely");
        ImGui::TableSetColumnIndex(1);
        if (GetGame().state_.players_[1][0] != '\0')
            ImGui::Text("%s", GetGame().state_.players_[1]);
        else
            ImGui::Text("...");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%s", PlayerStateStr(GetGame().state_.player_states_[1]));

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Grana");
        ImGui::TableSetColumnIndex(1);
        if (GetGame().state_.players_[2][0] != '\0')
            ImGui::Text("%s", GetGame().state_.players_[2]);
        else
            ImGui::Text("...");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%s", PlayerStateStr(GetGame().state_.player_states_[2]));

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Slabo");
        ImGui::TableSetColumnIndex(1);
        if (GetGame().state_.players_[3][0] != '\0')
            ImGui::Text("%s", GetGame().state_.players_[3]);
        else
            ImGui::Text("...");
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%s", PlayerStateStr(GetGame().state_.player_states_[3]));

        ImGui::EndTable();
    }

    ImGui::InputText("Server IP", g_ip_server, IM_ARRAYSIZE(g_ip_server));
    ImGui::SameLine();
    if (ImGui::Button("Connect"))
    {
        g_connection.connect(g_ip_server); // Connect to server
    }
    ImGui::SameLine();
    if (g_connection.connected())
    {
        ImGui::Text("(Now connected)");
    }
    else
    {
        ImGui::Text("(Now disconnected)");
    }
    ImGui::InputText("Nickname (max 15 chars)", g_nickname, IM_ARRAYSIZE(g_nickname));
    ImGui::SameLine();
    if (ImGui::Button("Play"))
    {
        if (g_nickname[0] == '\0')
        {
            LEEP_WARNING("You must set a Nickname before attempting to play.");
        }
        else
        {
            if (g_connection.connected())
            {
                ConPkg pkg;
                pkg.type_ = ConDataType::PLAY;
                strncpy_s(pkg.data_.log_, g_nickname, 16);
                g_connection.send(pkg);
            }
            else
            {
                LEEP_WARNING("Connect to the server first.");
            }
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Ready"))
    {
        ConPkg pkg;
        pkg.type_ = ConDataType::START;
        g_connection.send(pkg);
    }
    ImGui::End();
}

void leep::Init()
{
    {
        // Resource creation CPU side (GPU allocation will occur on first use)
        ResourceMap &rm = GM.resource_map();
        const String tp = "../assets/tex/";
        rm.addTexture("Default", "../assets/tex/default.jpg", TexType::SRGB);
        rm.addTexture("Skybox", 1024.0f, 1024.0f, TexType::ENVIRONMENT);
        rm.addTexture("IrradianceEnv", 1024.0f, 1024.0f, TexType::ENVIRONMENT);
        rm.addTexture("PrefilterSpec", 128.0f, 128.0f, TexType::PREFILTER_ENVIRONMENT);
        rm.addTexture("LutMap", 512.0f, 512.0f, TexType::LUT);
        // I need to load this as obj in order to normalmaps to work (bitangent etc.)
        rm.addGeometry("CubeObj", "../assets/obj/cube.obj");
        rm.addGeometry("SphereObj", "../assets/obj/sphere.obj");
        rm.addTextureAsync("CelticGold_A" ,tp + "celtic-gold/celtic-gold_A.png", TexType::SRGB);
        rm.addTextureAsync("CelticGold_N" ,tp + "celtic-gold/celtic-gold_N.png", TexType::RGB);
        rm.addTextureAsync("CelticGold_M" ,tp + "celtic-gold/celtic-gold_M.png", TexType::R);
        rm.addTextureAsync("CelticGold_R" ,tp + "celtic-gold/celtic-gold_R.png", TexType::R);

        rm.addTextureAsync("Peeling_A" ,tp + "peeling/peeling_A.png", TexType::SRGB);
        rm.addTextureAsync("Peeling_N" ,tp + "peeling/peeling_N.png", TexType::RGB);
        rm.addTextureAsync("Peeling_M" ,tp + "peeling/peeling_M.png", TexType::R);
        rm.addTextureAsync("Peeling_R" ,tp + "peeling/peeling_R.png", TexType::R);

        rm.addTextureAsync("Granite_A" ,tp + "granite/granite_A.png", TexType::SRGB);
        rm.addTextureAsync("Granite_N" ,tp + "granite/granite_N.png", TexType::RGB);
        rm.addTextureAsync("Granite_M" ,tp + "granite/granite_M.png", TexType::R);
        rm.addTextureAsync("Granite_R" ,tp + "granite/granite_R.png", TexType::R);

        rm.addTextureAsync("Slabs_A" ,tp + "tiles/tiles_A.png", TexType::SRGB);
        rm.addTextureAsync("Slabs_N" ,tp + "tiles/tiles_N.png", TexType::RGB);
        rm.addTextureAsync("Slabs_M" ,tp + "tiles/tiles_M.png", TexType::R);
        rm.addTextureAsync("Slabs_R" ,tp + "tiles/tiles_R.png", TexType::R);
    }

    memset(g_nickname, '\0', 16);
    GM.scene().createContainer(EntityType::RENDERABLE);
    GM.scene().createContainer(EntityType::RENDERABLE_VEL);
    GM.ui_tools().set_callback(&ConnectWindow);

    PbrData mat_data;
    mat_data.color_ = glm::vec3(1.0f, 0.0f, 0.0f);
    mat_data.use_albedo_map_ = 1.0f;
    mat_data.use_pbr_maps_ = 1.0f;
    mat_data.tiling_x_ = 1.0f;
    mat_data.tiling_y_ = 1.0f;
    mat_data.metallic_ = 0.5f;
    mat_data.roughness_ = 0.4f;
    mat_data.normal_map_intensity_ = 1.0f;

    GetGame().data_.mat_cubes_[0].set_type(MaterialType::MT_PBR);
    GetGame().data_.mat_cubes_[0].set_data(mat_data);
    GetGame().data_.mat_cubes_[0].set_albedo   (GM.resource_map().getTexture("CelticGold_A"));
    GetGame().data_.mat_cubes_[0].set_metallic (GM.resource_map().getTexture("CelticGold_M"));
    GetGame().data_.mat_cubes_[0].set_roughness(GM.resource_map().getTexture("CelticGold_R"));
    GetGame().data_.mat_cubes_[0].set_normal   (GM.resource_map().getTexture("CelticGold_N"));

    mat_data.tiling_x_ = 0.3f;
    mat_data.tiling_y_ = 0.3f;
    mat_data.normal_map_intensity_ = 0.6f;
    GetGame().data_.mat_cubes_[1].set_type(MaterialType::MT_PBR);
    GetGame().data_.mat_cubes_[1].set_data(mat_data);
    GetGame().data_.mat_cubes_[1].set_albedo   (GM.resource_map().getTexture("Peeling_A"));
    GetGame().data_.mat_cubes_[1].set_metallic (GM.resource_map().getTexture("Peeling_M"));
    GetGame().data_.mat_cubes_[1].set_roughness(GM.resource_map().getTexture("Peeling_R"));
    GetGame().data_.mat_cubes_[1].set_normal   (GM.resource_map().getTexture("Peeling_N"));

    mat_data.tiling_x_ = 0.3f;
    mat_data.tiling_y_ = 0.3f;
    mat_data.normal_map_intensity_ = 1.0f;
    GetGame().data_.mat_cubes_[2].set_type(MaterialType::MT_PBR);
    GetGame().data_.mat_cubes_[2].set_data(mat_data);
    GetGame().data_.mat_cubes_[2].set_albedo   (GM.resource_map().getTexture("Granite_A"));
    GetGame().data_.mat_cubes_[2].set_metallic (GM.resource_map().getTexture("Granite_M"));
    GetGame().data_.mat_cubes_[2].set_roughness(GM.resource_map().getTexture("Granite_R"));
    GetGame().data_.mat_cubes_[2].set_normal   (GM.resource_map().getTexture("Granite_N"));

    mat_data.tiling_x_ = 0.5f;
    mat_data.tiling_y_ = 0.5f;
    GetGame().data_.mat_cubes_[3].set_type(MaterialType::MT_PBR);
    GetGame().data_.mat_cubes_[3].set_data(mat_data);
    GetGame().data_.mat_cubes_[3].set_albedo   (GM.resource_map().getTexture("Slabs_A"));
    GetGame().data_.mat_cubes_[3].set_metallic (GM.resource_map().getTexture("Slabs_M"));
    GetGame().data_.mat_cubes_[3].set_roughness(GM.resource_map().getTexture("Slabs_R"));
    GetGame().data_.mat_cubes_[3].set_normal   (GM.resource_map().getTexture("Slabs_N"));

    mat_data.color_ = glm::vec3(1.0f, 0.0f, 0.0f);
    mat_data.use_albedo_map_ = 0.0f;
    mat_data.use_pbr_maps_ = 0.0f;
    mat_data.tiling_x_ = 1.0f;
    mat_data.tiling_y_ = 1.0f;
    mat_data.metallic_ = 0.6f;
    mat_data.roughness_ = 0.1f;
    mat_data.normal_map_intensity_ = 0.0f;
    GetGame().data_.mat_ball_.set_type(MaterialType::MT_PBR);
    GetGame().data_.mat_ball_.set_data(mat_data);

    // Cube
    {
        Entity e = Entity::CreateEntity("Player", EntityType::RENDERABLE_VEL);
        LTransform &tr = e.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(-3.0f, 0.0f, -5.0f));
        Drawable &dw = e.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("CubeObj");
        dw.material_ = GetGame().data_.mat_cubes_[1];
    }

    // Projectiles
    for (int32_t i = 0; i < 30; ++i)
    {
        Entity p = Entity::CreateEntity("Ball_" + ToString(i), EntityType::RENDERABLE_VEL);
        LTransform &tr = p.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(30.0f, 0.0f, -5.0f));
        tr.transform_ = glm::scale(tr.transform_, glm::vec3(0.3f, 0.3f, 0.3f));
        Drawable &dw = p.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("SphereObj");
        dw.material_ = GetGame().data_.mat_ball_;
    }

    // Floor
    mat_data.color_ = glm::vec3(0.6f, 0.6f, 0.6f);
    mat_data.use_albedo_map_ = 0.0f;
    mat_data.use_pbr_maps_ = 0.0f;
    mat_data.tiling_x_ = 1.0f;
    mat_data.tiling_y_ = 1.0f;
    mat_data.metallic_ = 0.1f;
    mat_data.roughness_ = 0.1f;
    mat_data.normal_map_intensity_ = 0.0f;
    {
        Entity floor = Entity::CreateEntity("Floor", EntityType::RENDERABLE);
        LTransform& tr = floor.getComponent<LTransform>();
        tr.transform_ = glm::translate(tr.transform_, glm::vec3(0.0f, -2.0f, -5.0f));
        tr.transform_ = glm::scale(tr.transform_, glm::vec3(20.0f, 1.0f, 3.0f));
        Drawable& dw = floor.getComponent<Drawable>();
        dw.geometry_ = GM.resource_map().getGeometry("Cube");
        dw.material_.set_type(MaterialType::MT_PBR);
        dw.material_.set_data(mat_data);
    }

    DisplayList init_dl;
    init_dl.addCommand<RenderOptions>()
        .enable_depth_test(true)
        .enable_write_depth(true)
        .set_cull_face(CullFace::BACK)
        .enable_blend(true)
        .set_blend_func(BlendFunc::ONE, BlendFunc::ZERO);
    init_dl.addCommand<EquirectangularToCubemap>()
        .set_in_path("../assets/tex/env/g-canyon-env.hdr")
        .set_out_cube(GM.resource_map().getTexture("Skybox"))
        .set_out_prefilter(GM.resource_map().getTexture("PrefilterSpec"))
        .set_out_lut(GM.resource_map().getTexture("LutMap"));
    init_dl.addCommand<EquirectangularToCubemap>()
        .set_in_path("../assets/tex/env/g-canyon-dif.hdr")
        .set_out_cube(GM.resource_map().getTexture("IrradianceEnv"));
    init_dl.submit();
}

void leep::Logic()
{
    if (GetGame().new_match_)
    {
        GetGame().new_match_ = false;
        Entity::GetEntity("Player").getComponent<Drawable>().material_ =
            GetGame().data_.mat_cubes_[GetGame().player_];
        LTransform* tr = Entity::GetEntity("Ball_0").componentPtr<LTransform>();
        for (int32_t i = 0; i < BallSpawner::kBallCount; ++i)
        {
            (tr + i)->setLocation(glm::vec3(50.0f, 0.0f, -5.0f));
        }
    }

    if (GetGame().playing_)
    {
        GM.input().updateInput();
        //CameraMovement(1.0f, 1.0f).executeSystem();
        Entity::GetEntity("Player").getComponent<Velocity>().velocity_.y -= 20.0f * GM.delta_time();
        CubeController("Player", 7.0f, 40.0f).executeSystem();
        BallSpawner(0.8f).executeSystem();
        ApplyVelocity(GM.scene().container(EntityType::RENDERABLE_VEL)).executeSystem();
        UpdateTransform(GM.scene().container(EntityType::RENDERABLE_VEL)).executeSystem();
        UpdateTransform(GM.scene().container(EntityType::RENDERABLE)).executeSystem();
        UpdateSceneGraph().executeSystem();
        PlayerCollision(&g_connection).executeSystem();
    }

    // Render commands
    DisplayList dl;
    PbrSceneData pbr_sd;
    pbr_sd.view_projection_ = GM.camera().view_projection();
    pbr_sd.camera_position_ = GM.camera().position();
    pbr_sd.light_direction_intensity_ = glm::vec4(0.0f, 1.0f, 0.0f, 0.1f);

    Material full_screen_img;
    full_screen_img.set_type(MaterialType::MT_FULL_SCREEN_IMAGE);
    full_screen_img.set_albedo(GM.camera().framebuffer().color());
    
    dl.addCommand<UseFramebuffer>()
        .set_framebuffer(GM.camera().framebuffer());

    dl.addCommand<RenderOptions>()
        .enable_depth_test(true)
        .enable_write_depth(true)
        .set_cull_face(CullFace::BACK)
        .enable_blend(true)
        .set_blend_func(BlendFunc::ONE, BlendFunc::ZERO);

    dl.addCommand<Clear>()
        .set_clear_buffer(true, true, false)
        .set_clear_color(0.2f, 0.2f, 0.2f, 1.0f);

    dl.addCommand<UsePbrMaterial>()
        .set_scene_data(pbr_sd)
        .set_irradiance_map(GM.resource_map().getTexture("IrradianceEnv"))
        .set_prefilter_map(GM.resource_map().getTexture("PrefilterSpec"))
        .set_lut_map(GM.resource_map().getTexture("LutMap"));

    dl.submit();

    Render(GM.scene().container(EntityType::RENDERABLE)).executeSystem();
    Render(GM.scene().container(EntityType::RENDERABLE_VEL)).executeSystem();
    
    dl.addCommand<RenderOptions>()
        .set_cull_face(CullFace::DISABLED);
    dl.addCommand<UseSkyboxMaterial>();
    dl.addCommand<DrawSkybox>()
        .set_cubemap(GM.resource_map().getTexture("Skybox"));
   
    dl.addCommand<UseFramebuffer>();

    dl.addCommand<RenderOptions>()
        .enable_depth_test(false);

    dl.addCommand<Clear>()
        .set_clear_buffer(true, false, false)
        .set_clear_color(1.0f, 0.0f, 0.0f, 1.0f);

    dl.addCommand<Draw>()
        .set_geometry(GM.resource_map().getGeometry("Quad"))
        .set_material(full_screen_img);
        
    dl.submit();


    g_connection.dispatchRecv();
}

void leep::Close()
{
    g_connection.disconnect();
}
