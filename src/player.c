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
#include "gfc_audio.h"


#include "gf3d_camera.h"
#include "player.h"

static int thirdPersonMode = 1;
static int layer = 1;
static int delay = 0;
static int frame = 0;
static int timerAnim = 0;
static int inv = 0;
//static int ground = 0;
static int damageON = 0;
static int multiplier = 1;
static int dashTime=0;  


void player_think(Entity *self);
void player_update(Entity *self);






Entity *player_new(Vector3D position)
{
    gfc_audio_init(1,1,1,1,1,1);
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

    //ent->position.z = 0;
    ent->type = 1;
    ent->ground = 0;
    ent->radius = 1;
    ent->health = 5;

    ent->ability = 0;
    ent->score = 0;
    return ent;
 

}

void abilityHandler(Entity* self) {
    switch (self->ability) {
    case 1: {break; }//FIRE
    case 2: {break; }//SPEED/INVINCIBILITY
    case 3: {break; }
    case 4: {break; }
    case 5: {break; }
    default:break;
    }
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
    const Uint8* press;
    keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
    press = SDL_GetKeyboardFocus(NULL);
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

    Sound* sfx = gfc_sound_load("audio/cartoon-jump-6462.wav",.5,0);
    //Sound* gameOver = gfc_sound_load("audio/negative_beeps-6008.wav", .5, 1);

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
        //vector3d_add(self->position, self->position, right);
        self->velocity.x = .25*multiplier;
    }else if (keys[SDL_SCANCODE_A])
    {
        //vector3d_add(self->position, self->position, -right);
        self->velocity.x = -.25*multiplier;
    }else { self->velocity.x = 0; }


    //space is jump
    if (keys[SDL_SCANCODE_SPACE]&&self->ability!=2) {
        //plays on button release
        //FIX THIS TODO
        gfc_sound_play(sfx,0,1,-1,-1);



        //vector3d_add(self->position, self->position);`
        //elf->position.z += self->position.z * .1;

        //FIX IT SO IT IS BETTER
        self->position.z += .75;//Adds height slower than the acceleration downwards, creating a jumpind effect
        //slog("%f", self->velocity.z);
        //self->velocity.z = 
        // 
        // USE kinematic equation
        // 
        // velocity = velocity + Acceleration*time;
        //self->velocity.z = .75 - self->acceleration.z * ;
        
        //PLAYER IS ALWAYS FAST FALLING FIX IT NOW
        
        //self->velocity.z = .005 +self->acceleration.z*.1;
        /*
        if (self->velocity.z == 0) {
            self->velocity.z =5;
        }
        */

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

    if (keys[SDL_SCANCODE_W]) {
        //ABILITY BUTTON
        if (self->score == 1) {
            dashTime = 50;
            if (keys[SDL_SCANCODE_A]) {
                //DASH LEFT
                
                for (int i = 0; i < 5; i++) {
                    self->gravity = NULL;
                    vector3d_add(self->position, self->position, -right);


                }
                self->acceleration.z = -0.05;
            }
            else {
                //DEFAULT IS RIGHT
                for (int i = 0; i < 5; i++) {
                    self->gravity = NULL;
                    vector3d_add(self->position, self->position, right);

                }
                self->acceleration.z = -0.05;
            }
        }

        if (self->score == 2) {
            self->position.y *= -1;
        }

        if (self->score == 3) {
            self->acceleration.z *= -1;
        }
    }

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



    //TODO ADD INVINCIBILLITY

    if (entityCollide(self) == 2 && delay == 0&&damageON==0) {
        //slog("collision detected");
        self->health -= 1;
        //slog("health decrement ");
       // slog(self->health);
        delay = 100;
        //slog(delay);

    }

    if (self->health <= 0) {
        //gfc_sound_play(gameOver, 0, 1, -1, -1);
        gfc_sound_play(sfx, 0, 1, -1, -1);
        slog("dead ");
        entity_free(self);
    }


    switch (self->ability) {
        case 1: {multiplier = 4; break; }//FIRE
        case 2: {self->rotation.z += 1; break; }//SPEED/INVINCIBILITY// FLIGHT
        case 3: {break; }
        case 4: {break; }
        case 5: {break; }
    default:break;
    }

    if (damageON == 1&&inv==0) {
        damageON == 0;
    }
    if (inv > 0) {
        inv--;
        slog(inv);
    }

    switch (entityCollide(self)) {
        case 3: {
            //PICKUP
            self->score += 1; //slog(self->score); 
            break;
            }
        case 4: {
            //HEALTH
            //ADD 1 Health if not at max health.
            //DONT GIVE HEALTH IF AT 5 or 10. 
                if (self->health<5) {
                    self->health++;
                }
                //only hit if 6 or up but less than 10 aka DOUBLE HEALTH BAR
                else if (self->health < 10 && self->health != 5) {
                    self->health++;
                }

                break;
            }
        case 5: {
            //SECOND BAR HEALTH
            self->health = 10;
            break;
            }
        case 6: {
            //FIRE
            self->ability = 1;
            slog("fire");
            break;
            }
        case 7: {
            //SPEED/INVINCIBILITY
            self->ability = 2;
            slog("speed");
            break;
            }
        default:break;
    }







    if (timerAnim == 0&&frame==1) {
        gf3d_model_free(self->model);

        self->model = gf3d_model_load("models/frog.model");
        frame = 0;
        timerAnim = 100;
    }
    if (timerAnim == 0 && frame == 0) {
        gf3d_model_free(self->model);

        self->model = gf3d_model_load("models/f1.model");
        frame = 1;
        timerAnim = 100;
    }

    timerAnim--;
  //  healthDisplay(self);
    /*
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
    */

    /*
    if (self->position.z != 0 && frame == 0) {
        gf3d_model_free(self->model);

        self->model = gf3d_model_load("models/frog.model");
        frame = 1; 

    }
    else if (self->position.z != 0 && frame==1) {
        gf3d_model_free(self->model);

        frame = 2;
        self->model = gf3d_model_load("models/f1.model");
    }*/
    /*
    else if (self->velocity.z == 0 && frame !=0) {

        //TODO create a state for player where they are not falling
        gf3d_model_free(self->model);

        self->model = gf3d_model_load("models/frog.model");
        frame=0;
    }*/
    
    
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
