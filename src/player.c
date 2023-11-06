#include "simple_logger.h"
#include "gfc_types.h"


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



#include "gf3d_camera.h"
#include "player.h"

static int thirdPersonMode = 1;
static int layer = 1;
static int delay = 0;
//static int ground = 0;

void player_think(Entity *self);
void player_update(Entity *self);






Entity *player_new(Vector3D position)
{
    Entity *ent = NULL;
    //double jumpHeight = 5;
    
    ent = entity_new();
    if (!ent)
    {
        slog("UGH OHHHH, no player for you!");
        return NULL;
    }
    
    ent->model = gf3d_model_load("models/frog.model");
    ent->think = player_think;
    ent->update = player_update;
    vector3d_copy(ent->position,position);

    //*****ROTATION IS IN RADIANS
    
    //gf3d_camera_set_position(ent->position);
    ent->rotation.x = -GFC_PI;//turns cammera rightside up
    //ent->rotation.z = -GFC_HALF_PI; 
    //ent->hidden = 1;
    ent->hidden = 0;

    ent->bounds.x = -2;
    ent->bounds.y = -2;
    ent->bounds.z = -2;
    ent->bounds.h = 4;
    ent->bounds.w = 4;
    ent->bounds.d = 4;

    ent->position.z = 0;
    ent->type = 1;
    ent->ground = 0;
    ent->radius = 1;
    ent->health = 5;


    return ent;
 

}


void healthDisplay(Entity* self) {
    gf3d_vgraphics_render_start();

   // gf2d_draw_rect_filled(gfc_rect(10, 10, 1000, 32), gfc_color8(128, 128, 128, 255));
    
    
    gf2d_font_draw_line_tag("5 HP", FT_H1, gfc_color(1, 1, 1, 1), vector2d(10, 10));

   // gf2d_draw_rect(gfc_rect(10, 10, 1000, 32), gfc_color8(255, 255, 255, 255));

    //gf2d_sprite_draw(mouse, vector2d(mousex, mousey), vector2d(2, 2), vector3d(8, 8, 0), gfc_color(0.3, .9, 1, 0.9), (Uint32)mouseFrame);
    gf3d_vgraphics_render_end();
}

void player_think(Entity* self)
{
    Vector3D forward = { 0 };
    Vector3D right = { 0 };
    //Vector3D camera;
    Vector2D w, mouse;
    int mx, my;
    SDL_GetRelativeMouseState(&mx, &my);
    const Uint8* keys;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
   // int delay = 0;
    mouse.x = mx;
    mouse.y = my;
    w = vector2d_from_angle(self->rotation.z);
    forward.x = w.x;
    forward.y = w.y;
    w = vector2d_from_angle(self->rotation.z - GFC_HALF_PI);
    right.x = w.x;
    right.y = w.y;
    gravity(self);
    //boundary(self);




    //controls commenting out the w s andothers
    /*
    if (keys[SDL_SCANCODE_W])
    {
        vector3d_add(self->position,self->position,forward);
    }
    if (keys[SDL_SCANCODE_S])
    {
        vector3d_add(self->position,self->position,-forward);
    }
    */

    if (keys[SDL_SCANCODE_D])
    {
        vector3d_add(self->position, self->position, right);
    }
    if (keys[SDL_SCANCODE_A])
    {
        vector3d_add(self->position, self->position, -right);
    }

    //space is jump
    if (keys[SDL_SCANCODE_SPACE]) {
        //vector3d_add(self->position, self->position);`
        //elf->position.z += self->position.z * .1;

        //FIX IT SO IT IS BETTER
        self->position.z += .75;//Adds height slower than the acceleration downwards, creating a jumpind effect
        
        //slog("JUMPING");

    }
   /*
    if (keys[SDL_SCANCODE_P]) {
        int x = self->position.x;
        slog( x);
        int y = self->position.y;
        slog(y);
        int z = self->position.z;
        slog(z);
    }
    */

    if (keys[SDL_SCANCODE_Z])self->position.y = -30;
    if (keys[SDL_SCANCODE_X])self->position.y = 0;

    //if (keys[SDL_SCANCODE_M]){ self->position.y += 100; }
    //if (keys[SDL_SCANCODE_K]) { self->position.y -= 100;}
    /*
    if (keys[SDL_SCANCODE_UP])self->rotation.x -= 0.0050;
    if (keys[SDL_SCANCODE_DOWN])self->rotation.x += 0.0050;
    if (keys[SDL_SCANCODE_RIGHT])self->rotation.z -= 0.0050;
    if (keys[SDL_SCANCODE_LEFT])self->rotation.z += 0.0050;
    
    if (mouse.x != 0)self->rotation.z -= (mouse.x * 0.001);
    if (mouse.y != 0)self->rotation.x += (mouse.y * 0.001);
    */

    if (keys[SDL_SCANCODE_F3])
    {
        thirdPersonMode = !thirdPersonMode;
        self->hidden = !self->hidden;
    }

    //vector3d_copy(camera, self->position);
    
    //for (i = 0, i < entity_manager.entity_count; i++) {

    //}

    /*
    for (j = 0; j < entity_manager.entity_count; j++) {
        if (i == j) {
            continue;
        }
        float deltaX = entity_manager.entity_list[i].position.x - self->position.x;
        float deltaZ = entity_manager.entity_list[i].position.z - self->position.z;
        float distance = deltaX * deltaX + deltaZ * deltaZ;
        //TO DO incorporate enemy radius
        if (distance <= (self->radius * self->radius)) {
            slog("collision here");
        }
       
    }*/
    //


    if (delay > 0) {
        //slog("delay decrement ");
        delay--;
    }


    if (entityCollide(self) == 0 && delay == 0) {
        //slog("collision detected");
        self->health -= 1; 
        //slog("health decrement ");
       // slog(self->health);
        delay = 100;
        //slog(delay);
    }




    if (self->health<=0) {
        slog("dead ");
        entity_free(self);
    }


  //  healthDisplay(self);
    
    if (self->position.x > 162) {
        self->velocity.x = 0;
        self->position.x = 162;
        slog("boundary hit");
    }
    if (self->position.x < -162) {
        self->velocity.x = 0;
        self->position.x = -162;
        slog("boundary hit");
    }

}



void player_update(Entity *self)
{
    Vector3D forward = {0};
    Vector3D position;
    Vector3D rotation;
    Vector2D w;
    
    if (!self)return;
    
    vector3d_copy(position,self->position);
    vector3d_copy(rotation,self->rotation);
    if (thirdPersonMode)
    {
        //position.z += 100;
        //position. y-= 100;
        //position.x += 20;
        //rotation.x += M_PI*0.125;
        w = vector2d_from_angle(self->rotation.z);
        forward.x = w.x * 100;
        forward.y = w.y * 100;
        vector3d_add(position,position,-forward);
    }
    gf3d_camera_set_position(position);
    gf3d_camera_set_rotation(rotation);
   

}



/*eol@eof*/
