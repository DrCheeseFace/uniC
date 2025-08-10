
struct MyStruct { /* ... */
};
typedef struct { /* ... */
} MyTypedefStruct;

struct mystruct {
} mystruct;

typedef struct AnotherStruct { /* ... */
} AnotherTypedefStruct;

struct TES_Case {};

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
