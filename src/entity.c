#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"

#include "entity.h"


typedef struct
{
    Entity* entity_list;
    Uint32  entity_count;
    Model* cube;
}EntityManager;

static EntityManager entity_manager = { 0 };


void entity_system_close()
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        entity_free(&entity_manager.entity_list[i]);        
    }
    free(entity_manager.entity_list);
    memset(&entity_manager,0,sizeof(EntityManager));
    slog("entity_system closed");
}

void entity_system_init(Uint32 maxEntities)
{
    entity_manager.entity_list = gfc_allocate_array(sizeof(Entity),maxEntities);
    if (entity_manager.entity_list == NULL)
    {
        slog("failed to allocate entity list, cannot allocate ZERO entities");
        return;
    }
    entity_manager.entity_count = maxEntities;
    atexit(entity_system_close);
    slog("entity_system initialized");
}

Entity *entity_new()
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)// not used yet, so we can!
        {
            entity_manager.entity_list[i]._inuse = 1;
            gfc_matrix_identity(entity_manager.entity_list[i].modelMat);
            entity_manager.entity_list[i].scale.x = 1;
            entity_manager.entity_list[i].scale.y = 1;
            entity_manager.entity_list[i].scale.z = 1;
            
            entity_manager.entity_list[i].color = gfc_color(1,1,1,1);
            entity_manager.entity_list[i].selectedColor = gfc_color(1,1,1,1);
            
            return &entity_manager.entity_list[i];
        }
    }
    slog("entity_new: no free space in the entity list");

    return NULL;
}

void entity_free(Entity *self)
{
    if (!self)return;
    //MUST DESTROY
    gf3d_model_free(self->model);
    memset(self,0,sizeof(Entity));
}


void entity_draw(Entity *self)
{
    if (!self)return;
    if (self->hidden)return;
    gf3d_model_draw(self->model,self->modelMat,gfc_color_to_vector4f(self->color),vector4d(1,1,1,1));
    if (self->selected)
    {
        gf3d_model_draw_highlight(
            self->model,
            self->modelMat,
            gfc_color_to_vector4f(self->selectedColor));
    }
   // gf3d_model_draw_highlight(self->model, self->modelMat, gfc_color_to_vector4(self->selectedColor));
      
}

void entity_draw_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)// not used yet
        {
            continue;// skip this iteration of the loop
        }
        entity_draw(&entity_manager.entity_list[i]);
    }
}

//0 ground 1 no enemy 2 ground 3 4 5 6 7 8 items, 

int entityCollideOLD(Entity* self) {
    int i;
    //slog("Collide function entered");
    for (i = 0; i < entity_manager.entity_count; i++)
    {
       // slog("For loop entered");
        if (self->type != entity_manager.entity_list[i].type && entity_manager.entity_list[i].type ==3 && self->type != 0&& self->type<3) {//checks for self
            //slog("IN check");
            // For ITEM
            //Box A; A.d = 0; A.h = 0; A.w = 0; A.x = 0; A.y = 0; A.z = 0;
            //Box B; B.d = 0; B.h = 0; B.w = 0; B.x = 0; B.y = 0; B.z = 0;
            Box A = gfc_box(self->bounds.x, self->bounds.y, self->bounds.z, self->bounds.w, self->bounds.h, self->bounds.d);
            Box B = gfc_box(entity_manager.entity_list[i].bounds.x, entity_manager.entity_list[i].bounds.y, entity_manager.entity_list[i].bounds.z, entity_manager.entity_list[i].bounds.w, entity_manager.entity_list[i].bounds.h, entity_manager.entity_list[i].bounds.d);
            vector3d_add(A, A, entity_manager.entity_list[i].position);
            vector3d_add(B, B, self->position);
            if (gfc_box_overlap(A, B)) {
                //slog("COLLISION NOW");
                //TODO CODE IN DAMAGE HERE
                //TODO CODE IN Actual COllision
                return 3;
            }

        }
        else if (self->type!= entity_manager.entity_list[i].type && entity_manager.entity_list[i].type!=0&&self->type!=0 && self->type < 3) {//checks for self
            //slog("IN check");
            
            //Box A; A.d = 0; A.h = 0; A.w = 0; A.x = 0; A.y = 0; A.z = 0;
            //Box B; B.d = 0; B.h = 0; B.w = 0; B.x = 0; B.y = 0; B.z = 0;
            Box A = gfc_box(self->bounds.x, self->bounds.y, self->bounds.z, self->bounds.w, self->bounds.h, self->bounds.d);
            Box B = gfc_box(entity_manager.entity_list[i].bounds.x, entity_manager.entity_list[i].bounds.y, entity_manager.entity_list[i].bounds.z, entity_manager.entity_list[i].bounds.w, entity_manager.entity_list[i].bounds.h, entity_manager.entity_list[i].bounds.d);
            vector3d_add(A, A, entity_manager.entity_list[i].position);
            vector3d_add(B, B, self->position);
            if (gfc_box_overlap(A, B)) {
                //slog("COLLISION NOW");
                //TODO CODE IN DAMAGE HERE
                //TODO CODE IN Actual COllision
                return 0;
            }

        }
        else if (self->type != entity_manager.entity_list[i].type && entity_manager.entity_list[i].type == 0 && self->type != 0 && self->type < 3) {
            //IF IT IS ground
            Box A = gfc_box(self->bounds.x, self->bounds.y, self->bounds.z, self->bounds.w, self->bounds.h, self->bounds.d);
            Box B = gfc_box(entity_manager.entity_list[i].bounds.x, entity_manager.entity_list[i].bounds.y, entity_manager.entity_list[i].bounds.z, entity_manager.entity_list[i].bounds.w, entity_manager.entity_list[i].bounds.h, entity_manager.entity_list[i].bounds.d);
            vector3d_add(A, A, entity_manager.entity_list[i].position);
            vector3d_add(B, B, self->position);
            if (gfc_box_overlap(A, B)) {
                //slog("COLLISION NOW");
                //TODO CODE IN DAMAGE HERE
                //TODO CODE IN Actual COllision
                //self->velocity.x = 0;
                //self->velocity.y = 0;
                self->velocity.z = 0;
                return 2;
            } 
        }else {
            continue;
        }
    }
    //if false
    return 1;
}

//0 ground 1 no enemy 2 ground 3 4 5 6 7 8 items, 

int entityCollide(Entity* self) {
    int i;
    //slog("Collide function entered");
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        //IF self!=same type as entity  and entity is of this tye, and the 
        //PLAYER is 1 
        //GROUND is 0
        //ENEMY is 2
        //ITEMS are 3 up


        //I DONT Care if the entities are colliding, so long as they stand on plarforms
        //USING NESTED IF STATEMENTS< WHILE RUDEMENTARY< BETTER FOR ORGTANIZATION COMPARED TO ORIGINAL
        //EASIER TO MANAGER


        //STRUCTURE
        //IF NOT SAME
        //IF TYPE
        //IF COLLIDING
        //RETURN TYPE
        if (self->type != entity_manager.entity_list[i].type) {
            if (self->type == 1) {

                //PLAYER

                Box A = gfc_box(self->bounds.x, self->bounds.y, self->bounds.z, self->bounds.w, self->bounds.h, self->bounds.d);
                Box B = gfc_box(entity_manager.entity_list[i].bounds.x, entity_manager.entity_list[i].bounds.y, entity_manager.entity_list[i].bounds.z, entity_manager.entity_list[i].bounds.w, entity_manager.entity_list[i].bounds.h, entity_manager.entity_list[i].bounds.d);
                vector3d_add(A, A, entity_manager.entity_list[i].position);
                vector3d_add(B, B, self->position);
                if (gfc_box_overlap(A, B)) {
                    //slog("COLLISION NOW");
                    //TODO CODE IN DAMAGE HERE
                    //TODO CODE IN Actual COllision
                    //self->velocity.x = 0;
                    //self->velocity.y = 0;

                    //IF PLATFORM ELSE CONTINUE
                    if (entity_manager.entity_list[i].type == 0) {
                        self->velocity.z = 0;
                    }
                    //Retrun the type collided if
                    return(entity_manager.entity_list[i].type);
                }//OVERLAP CHECK END

            }else if (self->type == 2){
                //ENEMY
                if (entity_manager.entity_list[i].type == 0) {
                    //PLAYER
                    Box A = gfc_box(self->bounds.x, self->bounds.y, self->bounds.z, self->bounds.w, self->bounds.h, self->bounds.d);
                    Box B = gfc_box(entity_manager.entity_list[i].bounds.x, entity_manager.entity_list[i].bounds.y, entity_manager.entity_list[i].bounds.z, entity_manager.entity_list[i].bounds.w, entity_manager.entity_list[i].bounds.h, entity_manager.entity_list[i].bounds.d);
                    vector3d_add(A, A, entity_manager.entity_list[i].position);
                    vector3d_add(B, B, self->position);
                    if (gfc_box_overlap(A, B)) {
                        //slog("COLLISION NOW");
                        //TODO CODE IN DAMAGE HERE
                        //TODO CODE IN Actual COllision
                        //self->velocity.x = 0;
                        //self->velocity.y = 0;
                        self->velocity.z = 0;
                        //Retrun the type collided if
                        return(0);
                    }//OVERLAP CHECK END

                }

            }
            else if (self->type >= 3) {
                //ENEMY
                if (entity_manager.entity_list[i].type == 1) {
                    //PLAYER
                    Box A = gfc_box(self->bounds.x, self->bounds.y, self->bounds.z, self->bounds.w, self->bounds.h, self->bounds.d);
                    Box B = gfc_box(entity_manager.entity_list[i].bounds.x, entity_manager.entity_list[i].bounds.y, entity_manager.entity_list[i].bounds.z, entity_manager.entity_list[i].bounds.w, entity_manager.entity_list[i].bounds.h, entity_manager.entity_list[i].bounds.d);
                    vector3d_add(A, A, entity_manager.entity_list[i].position);
                    vector3d_add(B, B, self->position);
                    if (gfc_box_overlap(A, B)) {
                        //slog("COLLISION NOW");
                        //TODO CODE IN DAMAGE HERE
                        //TODO CODE IN Actual COllision
                        //self->velocity.x = 0;
                        //self->velocity.y = 0;

                        //Retrun the type collided if
                        slog("collected ");
                        

                        if (self->type == 3) {
                            //CAPPED SCIRE AT 3
                            if (entity_manager.entity_list[i].score < 3) {
                                entity_manager.entity_list[i].score += 1;
                            }
                        }
                        entity_free(self);

                        
                        
                        //return(self->type);
                    }//OVERLAP CHECK END

                }
            }//IF SELF IS TYPE
        }//IF NOT THE SAME

        
     }//For LOOP


}



void entity_think(Entity *self)
{
    if (!self)return;
    entityCollide(self);
    if (self->think)self->think(self);
    //MY STUFF
   // self->velocity.x += 1;
    //CAMERA is also an entity
    
}

void entity_think_all()
{
    int i, j;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)// not used yet
        {
            continue;// skip this iteration of the loop
        }
        /*

        for (j = 0; j < entity_manager.entity_count; j++) {
            if (i == j) {
                continue;
            }
            if (entity_collide_check(&entity_manager.entity_list[i], &entity_manager.entity_list[j]) == 1) {
             
            }
        }
        */
        entity_think(&entity_manager.entity_list[i]);
    }
}
void entity_generic_update(Entity* self)
{
    if (!self)
    {
        slog("self pointer not provided");
        return;
    }
    vector3d_add(self->position, self->position, self->velocity);
    // self->rotation.z += 1;//.01
     //self->rotation.y += 1;//.01
}

/*
int entity_collide_check(Entity* self, Entity* other) {
    if ((!self) || (!other)) {
        //nother to collide into
        slog("Missing entity to collide with");
        return 0;
    }

    //Box A; A.d = 0; A.h = 0; A.w = 0; A.x = 0; A.y = 0; A.z = 0;
    //Box B; B.d = 0; B.h = 0; B.w = 0; B.x = 0; B.y = 0; B.z = 0;
    Box A = gfc_box(self->bounds.x, self->bounds.y, self->bounds.z, self->bounds.w, self->bounds.h, self->bounds.d);
    Box B = gfc_box(other->bounds.x, other->bounds.y, other->bounds.z, other->bounds.w, other->bounds.h, other->bounds.d);
    vector3d_add(A, A, other->position);
    vector3d_add(B, B, other->position);
    return gfc_box_overlap(A, B);
}

Entity* entity_get_collision_entity(Entity* self) {
    int i=0, c, j;
    if (!self) {
        slog("no self provided");
    }
    //for (i = 0, i < entity_manager.entity_count; i++) {

    //}
    for (j = 0; j < entity_manager.entity_count; j++) {
        if (i == j) {
            continue;
        }
        if (entity_collide_check(&entity_manager.entity_list[i], &entity_manager.entity_list[j]) == 1) {
            
        }
    }

}

*/



void entity_update(Entity *self)
{
    if (!self)return;
    // HANDLE ALL COMMON UPDATE STUFF
    
    vector3d_add(self->position,self->position,self->velocity);
    vector3d_add(self->velocity,self->acceleration,self->velocity);
    
    gfc_matrix_identity(self->modelMat);
    
    gfc_matrix_scale(self->modelMat,self->scale);
    gfc_matrix_rotate_by_vector(self->modelMat,self->modelMat,self->rotation);
    gfc_matrix_translate(self->modelMat,self->position);
    
    if (self->update)self->update(self);
}

void entity_update_all()
{
    int i;
    for (i = 0; i < entity_manager.entity_count; i++)
    {
        if (!entity_manager.entity_list[i]._inuse)// not used yet
        {
            continue;// skip this iteration of the loop
        }
        entity_update(&entity_manager.entity_list[i]);
    }
}

void gravity(Entity* self) {
    
    if (self->position.z > self->ground) {
        self->acceleration.z = -.005;
    }
    else {
        self->velocity.z = 0;
    }
    
}

Entity* entity_spawn_from_config(SJson* config) {
    //Entity ent = NULL;
   // return ent;
}






/*eol@eof*/
