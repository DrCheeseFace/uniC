// DO NOT REFORMAT THIS FILE

struct MyStruct { /* ... */
int window_w;
};
typedef struct { /* ... */
int window_w;
} MyTypedefStruct;

struct mystruct {
int window_w;
} mystructtypedefname;

typedef struct{ // no gap
                int window_w;
} nogapstruct;

typedef struct AnotherStruct { /* ... */
int window_w;
} AnotherTypedefStruct;

struct TES_Case {int window_w;};

struct TES_Context {
	int pass_count;
	int fail_count;
};

struct G_GameState {
	int window_w;
	int window_h;
	float scale;
	double target_fps;
	double target_frametime_ms;
	struct {
		int tsumo;
		int riichi;
		int double_riichi;
		int ippatsu;
		int haitei;
		int chankan;
		int rinshan;
		int tenhou;
	} conditions;
	int selector_idx;
};

struct G_GameState bruh = { 0 }; // should not get picked up
struct G_GameState bruh; // should not get picked up
