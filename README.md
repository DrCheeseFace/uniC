# unic 
`CLI tool for checking my C naming conventions` WIP

## Rules
1) all variable names must be `snake_case` eg: `my_variable`
2) all `struct` `enum` `union` `typedef` must be `CamelCase` eg: `MyState`
3) all `#define` must be `SCREAMING_SNAKE_CASE`
4) all `enum` values must be `SCREAMING_SNAKE_CASE` and must be prefixed with the type name \
   eg: `enum MyEnum` -> `MY_ENUM_DEFAULT`
5) all function names are `snake_case` eg: `void my_function(void)`
6) for all source files with a prefix denoted with the first "_" 
    - the max prefix length is 5 character (prefer 1-3)
    - all `struct` `enum` `typedef` `define` `function` 's must be prefixed with the uppercase version of the prefix \
    eg: `s_this_state.h` -> `struct S_ThisState` `#define S_WINDOW_WIDTH` `S_calculate_this(int x)`


## Reference  

### file is not prefixed eg: `utils.h`
| type             | example             |
| ---------------- | ------------------- |
| variables        | is_open             |
| function         | my_function(void)   |
| struct           | MyGameState         |
| typedef          | Points              |
| enum             | MyEnum              |
| enum value       | MY_ENUM_DEFAULT     |
| union            | MyUnion             |
| definition/macro | WINDOW_WIDTH        |

### file is prefixed eg: `t_tiles.h`
| type             | example             |
| ---------------- | ------------------- |
| variables        | is_open             |
| function         | G_my_function(void) |
| struct           | G_MyGameState       |
| typedef          | G_Points            |
| enum             | G_MyEnum            |
| enum value       | G_MY_ENUM_DEFAULT   |
| union            | G_MyUnion           |
| definition/macro | G_WINDOW_WIDTH      |


## Examples
```c
# g_game_state.h

#ifndef G_GAME_STATE_H
#define G_GAME_STATE_H

#define G_MAX_HAND_TILE_COUNT 18

typedef enum {
	G_SELECTED_MAIN_MENU_OPTION_HAND = 0,
	G_SELECTED_MAIN_MENU_OPTION_COUNT,
} G_SelectedMainMenuOption;

struct G_GameState {
	int window_w;
	int window_h;
};

struct G_GameState *G_game_state_create(int target_fps, int window_width,
				       int window_height, float scale);

#endif // !G_GAME_STATE_H
```

```c
# g_game_state.c

struct G_GameState *G_game_state_create(int target_fps, int window_width,
				       int window_height, float scale)
{
	struct G_GameState *game_state = malloc(sizeof(struct G_GameState));
	game_state->window_w = window_width;
	game_state->window_h = window_height;

	G_clear_menus_state(game_state);

	return game_state;
}
```

```c
# utils.h

#ifndef UTIL_H
#define UTIL_H

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1000
#define WINDOW_SCALE (float)0.5
#define TARGET_FPS 35

#define MAX_ALLOCATED_TEXTURES 20

#define PLACEHOLDER_TEXT "ERR"

#define DEALER_SEAT T_TILE_TON

#define SCORE_SPACER "/"

#endif // !UTIL_H
```

```c
# utils.c

int point_distance(Point a, Point b)
{
    int distance;
    .
    .
    .
    return distance;
}
```

