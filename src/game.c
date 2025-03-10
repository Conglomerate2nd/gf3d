#include <SDL.h>            

#include "simple_logger.h"
#include "gfc_input.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"
#include "gf3d_model.h"
#include "gf3d_camera.h"
#include "gf3d_texture.h"
#include "gf3d_particle.h"

#include "gf2d_sprite.h"
#include "gf2d_font.h"
#include "gf2d_draw.h"
#include "gfc_audio.h"

#include "entity.h"
#include "agumon.h"
#include "player.h"
#include "world.h"
#include "pickup.h"

#include "simple_json_array.h"
#include "simple_json_object.h"

extern int __DEBUG;
static int scene = 0;

int main(int argc,char *argv[])
{
    int done = 0;
    int a;
    
    Sprite *mouse = NULL;
    int mousex,mousey;
    //Uint32 then;
    float mouseFrame = 0;
    World *w;
    Entity *agu;
    Entity* level;
    Entity* flyer,*flyer2;
    Entity* jumpy;
    Entity* boss;
    Entity* player;
    Particle particle[100];
    Matrix4 skyMat;
    Model *sky;
   
    //gfc_audio_init;
    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"--debug") == 0)
        {
            __DEBUG = 1;
        }
    }
    
    init_logger("gf3d.log",0);    
    gfc_input_init("config/input.cfg");
    slog("gf3d begin");
    gf3d_vgraphics_init("config/setup.cfg");
    gf2d_font_init("config/font.cfg");
    gf2d_draw_manager_init(1000);
    
    slog_sync();
    
    entity_system_init(1024);
    
    mouse = gf2d_sprite_load("images/pointer.png",32,32, 16);
    
    //ALL spawns
    //agu = agumon_new(vector3d(0 ,0,0));
    //level = level_new(vector3d(0,0,0));
    flyer = flyer_new(vector3d(40, 0, 10));
    flyer = flyer_new(vector3d(50, -30, 10));

    jumpy_new(vector3d(-40, 0, 0));
    boss_new(vector3d(-40, -30, 0));
    //boss_new(vector3d(40, -30, 0));
    platform_new(vector3d(40, -30, 20));
    pickup_new((vector3d(40, -30, 0)));
    pickup_new((vector3d(40, -30, 25)));
    pickup_new((vector3d(60, -30, 0)));
    fire_new((vector3d(-40, -30, 0)));
    double_health_new((vector3d(40, -30, 40)));
    health_up_new((vector3d(40, -30, 50)));
    health_up_new((vector3d(40, 0, 60)));

    speed_up_new((vector3d(70, 0, 0)));
    //if (agu)agu->selected = 1;
    w = world_load("config/testworld.json");
    //w = world_load("model/Leveltest.model");

    spawn_entity_by_name("flyer", vector3d(40, -30, 0));
    SDL_SetRelativeMouseMode(SDL_TRUE);
    slog_sync();
    gf3d_camera_set_scale(vector3d(1,1,1));
    player = player_new(vector3d(-50,0,0));
    /*
    for (a = 0; a < 100; a++)
    {
        particle[a].position = vector3d(gfc_crandom() * 100,gfc_crandom() * 100,gfc_crandom() * 100);
        particle[a].color = gfc_color(0,0,0,1);
        particle[a].color = gfc_color(gfc_random(),gfc_random(),gfc_random(),1);//orignally commented out
        particle[a].size = 100 * gfc_random();
    }*/
    a = 0;
    sky = gf3d_model_load("models/sky.model");
    gfc_matrix_identity(skyMat);
    gfc_matrix_scale(skyMat,vector3d(100,100,100));



    
    // main game loop
    slog("gf3d main loop begin");
    //slog(sj_object_to_json_string("config/testworld.json"));
    while (!done)
    {
        //ENTER MAIN MENU
        gfc_input_update();
        gf2d_font_update();
        SDL_GetMouseState(&mousex, &mousey);

        mouseFrame += 0.01;
        if (mouseFrame >= 16)mouseFrame = 0;
        world_run_updates(w);
        entity_think_all();
        entity_update_all();
        gf3d_camera_update_view();
        gf3d_camera_get_view_mat4(gf3d_vgraphics_get_view_matrix());


        //IF ANYTHING NEEDS TO BE DRAWN PU IT HERE
        gf3d_vgraphics_render_start();
        gf3d_model_draw_sky(sky, skyMat, gfc_color(1, 1, 1, 1));


        //3D draws
        if (scene == 0) {
            gf2d_sprite_draw(mouse, vector2d(mousex, mousey), vector2d(2, 2), vector3d(8, 8, 0), gfc_color(0.3, .9, 1, 0.9), (Uint32)mouseFrame);

           
            gf2d_draw_rect(gfc_rect(500, 500, 1000, 32), gfc_color8(255, 255, 255, 255));
            gf2d_font_draw_line_tag("Start", FT_H1, gfc_color(1, 1, 1, 1), vector2d(500, 500));

            gf2d_sprite_draw(mouse, vector2d(mousex, mousey), vector2d(2, 2), vector3d(8, 8, 0), gfc_color(0.3, .9, 1, 0.9), (Uint32)mouseFrame);

            if (mousex>= 500&&mousex<=1000 && mousey >= 500 && mousey <= 1000) {
                scene = 1;
            }

        }else {

        world_draw(w);
        entity_draw_all();

        for (a = 0; a < 100; a++)
        {
            gf3d_particle_draw(&particle[a]);
        }
        //2D draws
        gf2d_draw_rect_filled(gfc_rect(10, 10, 1000, 32), gfc_color8(128, 128, 128, 255));

        gf2d_font_draw_line_tag("HP", FT_H1, gfc_color(1, 1, 1, 1), vector2d(10, 10));

        gf2d_draw_rect(gfc_rect(10, 10, 1000, 32), gfc_color8(255, 255, 255, 255));

        gf2d_draw_rect_filled(gfc_rect(1000, 10, 1000, 32), gfc_color8(128, 128, 128, 255));
        gf2d_font_draw_line_tag("Power", FT_H1, gfc_color(1, 1, 1, 1), vector2d(1000, 10));

        switch (player->score) {
        case 3:gf2d_draw_rect_filled(gfc_rect(1000, 14, 32, 32), gfc_color8(0, 0, 255, 255));
        case 2:gf2d_draw_rect_filled(gfc_rect(1000, 30, 32, 32), gfc_color8(0, 0, 255, 255));
        case 1:gf2d_draw_rect_filled(gfc_rect(1000, 46, 32, 32), gfc_color8(0, 0, 255, 255));  break;
        case 0:  break;
        default:slog("error at drawing"); break;
        }




        //goes to the case and draws everything below it
        //10 is the bonus pickup
        switch (player->health) {
        case 10:gf2d_draw_rect_filled(gfc_rect(138, 50, 32, 32), gfc_color8(255, 0, 0, 255));
        case 9:gf2d_draw_rect_filled(gfc_rect(106, 50, 32, 32), gfc_color8(255, 0, 0, 255));
        case 8:gf2d_draw_rect_filled(gfc_rect(74, 50, 32, 32), gfc_color8(255, 0, 0, 255));
        case 7:gf2d_draw_rect_filled(gfc_rect(42, 50, 32, 32), gfc_color8(255, 0, 0, 255));
        case 6:gf2d_draw_rect_filled(gfc_rect(10, 50, 32, 32), gfc_color8(255, 0, 0, 255));
        case 5:gf2d_draw_rect_filled(gfc_rect(138, 30, 32, 32), gfc_color8(255, 0, 0, 255));
        case 4:gf2d_draw_rect_filled(gfc_rect(106, 30, 32, 32), gfc_color8(255, 0, 0, 255));
        case 3:gf2d_draw_rect_filled(gfc_rect(74, 30, 32, 32), gfc_color8(255, 0, 0, 255));
        case 2:gf2d_draw_rect_filled(gfc_rect(42, 30, 32, 32), gfc_color8(255, 0, 0, 255));
        case 1:gf2d_draw_rect_filled(gfc_rect(10, 30, 32, 32), gfc_color8(255, 0, 0, 255));  break;
        case 0:  break;
        default:slog("error at drawing"); break;
        }
    }
                
        gf3d_vgraphics_render_end();



        if (gfc_input_command_down("exit"))done = 1; // exit condition
    }    
    
    world_delete(w);
    
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    slog("gf3d program end");
    slog_sync();
    return 0;
}

/*eol@eof*/
