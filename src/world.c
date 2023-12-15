#include "simple_logger.h"
#include "simple_json.h"
#include "simple_json_list.h"
#include "simple_json_array.h"
#include "simple_json_string.h"

#include "gfc_types.h"
#include "gfc_config.h"

#include "world.h"


#include "spawns.h"




/*
typedef struct
{

    Model *worldModel;
    List *spawnList;        //entities to spawn
    List *entityList;       //entities that exist in the world
}World;
*/




World *world_load(char *filename)
{
    SJson* json, * wjson;
    SJson* array, * item;
    World *w = NULL;
    char* list;
    

    const char *modelName = NULL;
    w = gfc_allocate_array(sizeof(World),1);
    if (w == NULL)
    {
        slog("failed to allocate data for the world");
        return NULL;
    }
    json = sj_load(filename);
    if (!json)
    {
        slog("failed to load json file (%s) for the world data",filename);
        free(w);
        return NULL;
    }
    wjson = sj_object_get_value(json,"world");
    if (!wjson)
    {
        slog("failed to find world object in %s world condig",filename);
        free(w);
        sj_free(json);
        return NULL;
    }
    modelName = sj_get_string_value(sj_object_get_value(wjson,"model"));
    if (!modelName)
    {
        slog("world data (%s) has no model",filename);
        sj_free(json);
        return w;
    }
    w->model = gf3d_model_load(modelName);



    sj_value_as_vector3d(sj_object_get_value(wjson,"scale"),&w->scale);
    sj_value_as_vector3d(sj_object_get_value(wjson,"position"),&w->position);
    sj_value_as_vector3d(sj_object_get_value(wjson,"rotation"),&w->rotation);
    sj_free(json);
    w->color = gfc_color(1,1,1,1);


    array = gfc_allocate_array(sizeof(sj_object_get_value(wjson, "spawnList")), 1);
    
    array = sj_object_get_value(wjson, "spawnList");
    SJson* enemy;
     
    if (array) {

        slog(" IT DO WORK");
    }else{ slog("SHIT IT DONT WORK"); }


   spawn_entity_by_name("platform", vector3d(-40,-30,25));
    
   
    

    //IDEA CYCLE THROUGH ALL NAMES AND CHECK FOR EACH NAME
   


    return w;
}

void world_draw(World *world)
{
    if (!world)return;
    if (!world->model)return;// no model to draw, do nothing
    gf3d_model_draw(world->model,world->modelMat,gfc_color_to_vector4f(world->color),vector4d(2,2,2,2));
    //gf3d_model_draw_highlight(world->worldModel,world->modelMat,vector4d(1,.5,.1,1));
}

void world_delete(World *world)
{
    if (!world)return;
    gf3d_model_free(world->model);
    free(world);
}

void world_run_updates(World *self)
{
   // self->rotation.z += 0.0001;
    gfc_matrix_identity(self->modelMat);
    
    gfc_matrix_scale(self->modelMat,self->scale);
    gfc_matrix_rotate_by_vector(self->modelMat,self->modelMat,self->rotation);
    gfc_matrix_translate(self->modelMat,self->position);

}

void world_add_entity(World *world,Entity *entity){

}

int world_collision_height(Vector3D from, Vector3D* collide) {

}




/*eol@eof*/
