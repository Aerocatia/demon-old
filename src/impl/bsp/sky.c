#include <stdint.h>

#include "ringhopper/scenario.h"
#include "ringhopper/scenario_structure_bsp.h"
#include "ringhopper/sky.h"

#include "sky.h"
#include "collision.h"
#include "../id.h"
#include "../tag.h"

ScenarioStructureBSP **loaded_bsp_data = (ScenarioStructureBSP **)(0x7367BC);

uint32_t *current_leaf_index = (uint32_t *)(0x7B27E4);
uint32_t *current_cluster_index = (uint32_t *)(0x7B27E8);
uint16_t *current_sky_index = (uint16_t *)(0x7B27EE);
uint8_t *should_draw_skybox = (uint8_t *)(0x7B27ED);

void set_skybox_info(VectorXYZ *point) {
    // Find the leaf we're in. If our leaf is not null, we can use it.
    //
    // Otherwise, if it is null, we can use it only if our last leaf was invalid for our *CURRENT* BSP. This is
    // SUUUUUUUUUUUUPER hacky, but the whole BSP and skybox disappear if the camera goes out of bounds if you don't do
    // this in this exact way, and the camera goes out of bounds on several cutscenes. -.-'
    uint32_t new_leaf_index = collision_bsp_leaf_for_point((*loaded_bsp_data)->collision_bsp.elements, point, 0);

    if(new_leaf_index != 0xFFFFFFFF || *current_leaf_index >= (*loaded_bsp_data)->leaves.count) {
        *current_leaf_index = new_leaf_index;
    }

    // These parameters are safe.
    *current_sky_index = 0xFFFF;
    *should_draw_skybox = 0;

    // Resolve the current cluster index too.
    *current_cluster_index = bsp_cluster_for_leaf(*current_leaf_index);

    // No cluster? Do nothing then.
    if(*current_cluster_index == 0xFFFFFFFF) {
        return;
    }

    // Get the current sky index.
    *current_sky_index = (*loaded_bsp_data)->clusters.elements[*current_cluster_index].sky;

    // Special case for an index of 65535: it uses 'indoor' fog.
    if(*current_sky_index == 0xFFFF) {
        return;
    }

    // Get the scenario data
    Scenario *scenario_data = get_scenario_tag_data();

    // Otherwise, if this is an invalid skybox, it will just use no skybox (black) and no fog, which shouldn't even happen on valid tag data.
    if(*current_sky_index >= scenario_data->skies.count) {
        return;
    }

    TableID sky_tag_id = scenario_data->skies.elements[*current_sky_index].sky.tag_id;

    // Check if null
    // (the game also checks if the sky index is signed, but that's insane, plus it'd short circuit here)
    if(ID_IS_NULL(sky_tag_id)) {
        return;
    }

    // Check if skybox model is null
    if(ID_IS_NULL(((Sky *)(get_tag_data(sky_tag_id)))->model.tag_id)) {
        return;
    }

    *should_draw_skybox = 1;
}

uint32_t bsp_cluster_for_leaf(uint32_t leaf) {
    // Null leaf = null cluster
    if(leaf == 0xFFFFFFFF) {
        return 0xFFFFFFFF;
    }

    // It's worth noting that this check is NOT necessary if the tag data is valid.
    if(leaf >= (*loaded_bsp_data)->leaves.count) {
        return 0xFFFFFFFF;
    }

    return (*loaded_bsp_data)->leaves.elements[leaf].cluster;
}
