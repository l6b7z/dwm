/* dwm 6.2 */
#include "exitdwm.c"
#include "unfloat.c"
#include "tile_centre.c"
#include "tile_grid.c"
#include "tile_wide.c"

#define TERMINAL  "st"
#define BROWSER   "librewolf"
#define STATUSBAR "dwmblocks"
#define KBD_SCRIPT "/home/l6b7/.static/resources/kbd_script/"

static const int wrapCursor          = 1; /* wraps cursor to selected window */
static const int scrollsensetivity   = 30; /* 1 means resize window by 1 pixel for each scroll event */

static const int topbar              = 1;    /* bar position */
static const int showbar             = 1;
static const int user_bh             = 16.2; /* bar height */

static const int swallowfloating    = 1; /* 1 means swallow floating windows by default */
static const unsigned int snap      = 3; /* snap windows to edge of the screen or windows */
static const unsigned int borderpx  = 1;
static const unsigned int gappih    = 2;
static const unsigned int gappiv    = 2;
static const unsigned int gappoh    = 2;
static const unsigned int gappov    = 2;

static const char barBG[]           = "#000000";
static const char client[]          = "#000000";
static const char activeClient[]    = "#5F00FF";
static const char floatClient[]     = "#4D007D";
static const char barText[]         = "#5F00FF";

static const char *fonts[]           = { "IosevkaNerdFont:size=14.2",
                                         "noto-fonts-emoji:size=10"};
#include "util_cleanup.c" 

/*f  = float | float coordinates | t  = tag | n  = no swallow | t  = term | m  = mon*/
static const Rule rules[] = { 
//f, x___, y___, w___, h___, t, n, t, m, win, class________, title________},
 {0,  200,  300,  400,  500, 0, 0, 1,-1,NULL, "st"         , NULL         },
 {1,  200,  300,  400,  500, 0, 1, 1,-1,NULL, "st"         , "zz"         },
};

static const Layout layouts[] = {
  { "T",   tile }, 	
  { "G",   grid },	
	{ "W",   tilewide },
	{ "C",   tcl },
	{ NULL,  NULL },
}; 



//MODKEYS,            KEY,         function,          args, */
static Key keys[] = {
// testing
{ HYPER|SHIFT,        XK_period,  spawn,             {.v = (const char*[]){ "sys_clipmenu", NULL } } },
 // Actions
{ HYPER,              XK_q,       killclient,        {0} },
{ HYPER,              XK_b,       togglebar,         {0} },
{ HYPER|SHIFT,        XK_q,       exitdwm,           {0} },
// Terminal 
{ ALT,	              XK_space,   spawn,		         SHCMD("st -d $ZK_DIR_MAIN") },
// Browser
{ SUPER,              XK_space,   spawn,             {.v = (const char*[]){ BROWSER, NULL } } },
// File Browser
{ HYPER,              XK_e,       spawn,             {.v = (const char*[]){ "lf_main-dir", NULL } } }, /* lf file manager*/
{ HYPER|SHIFT,        XK_e,       spawn,             {.v = (const char*[]){ "lf_root", NULL } } },     /* lf file manager with root access*/
// Scripts (Sys Functionality)
{ HYPER,              XK_F8,      spawn,             {.v = (const char*[]){ "mon_select", NULL } } },  /* display settings*/
{ HYPER|SHIFT,        XK_comma,   spawn,             {.v = (const char*[]){ "emoji_select", NULL } } },
{ 0,                  XK_Print,   spawn,             {.v = (const char*[]){ "screenshot", NULL } } },
{ SHIFT,              XK_Print,   spawn,             {.v = (const char*[]){ "record_screen", NULL } } },
{ HYPER|SHIFT,        XK_p,       spawn,             {.v = (const char*[]){ "colorpicker_to_clipboard", NULL } } },
{ HYPER|SHIFT,        XK_y,       spawn,             {.v = (const char*[]){ "ytfzf_dmenu", NULL } } },
//layouts
{ HYPER|SHIFT,        XK_t,       cyclelayout,       {.i = +1 } },
{ HYPER,              XK_t,       unfloatvisible,    {.v = &layouts[0]} },
{ HYPER,              XK_f,       togglefullscreen,  {0} },
/* { HYPER,            XK_t,      setlayout,      {.v = &layouts[0]} }, */
/* { HYPER,            XK_g,      setlayout,      {.v = &layouts[1]} }, */
/* { HYPER|SHIFT,      XK_g,      setlayout,      {.v = &layouts[2]} }, */
// Window  actions
{ HYPER,              XK_Return,  zoom,              {0} },
{ HYPER|SHIFT,        XK_space,   togglefloating,    {0} },
{ HYPER|SHIFT,        XK_b,       toggleborder,      {0} },
{ HYPER|SHIFT,		    BRCKTL,	    spawn,	           SHCMD("transset-df -a --dec .0025") }, /* let's you change the..  */
{ HYPER|SHIFT,		    BRCKTR,	    spawn,	           SHCMD("transset-df -a --inc .0025") }, /* ..opacity of the window */
// resize master slave ratio
{ HYPER|SHIFT,        XK_Left,    setmfact,          {.f = -0.05} },
{ HYPER|SHIFT,        XK_Right,   setmfact,          {.f = +0.05} },
// increase master number
{ HYPER|SHIFT,        XK_Up,      incnmaster,        {.i = +1 } }, 
{ HYPER|SHIFT,        XK_Down,    incnmaster,        {.i = -1 } },
//Focusing Tags
{ HYPER,              XK_Tab,     view,              {0} }, //cycle last
{ HYPER,              BRCKTL,     viewprev,          {0} },
{ HYPER,              BRCKTR,     viewnext,          {0} },
//Focusing Windows
{ HYPER,              XK_r,       previewallwin,     {0} },/* windows preview (click to focus) */
{ SUPER,              XK_Left,    focusdir,          {.i = 0 } }, 
{ SUPER,              XK_Right,   focusdir,          {.i = 1 } }, 
{ SUPER,              XK_Up,      focusdir,          {.i = 2 } }, 
{ SUPER,              XK_Down,    focusdir,          {.i = 3 } }, 
{ HYPER,              XK_h,       focusdir,          {.i = 0 } }, 
{ HYPER,              XK_l,       focusdir,          {.i = 1 } }, 
{ HYPER,              XK_k,       focusdir,          {.i = 2 } }, 
{ HYPER,              XK_j,       focusdir,          {.i = 3 } }, 
//Window Pos
{ HYPER|ALT,          XK_Down,    moveresize,        {.v = "  0x  25y   0w   0h" } },
{ HYPER|ALT,          XK_Up,      moveresize,        {.v = "  0x -25y   0w   0h" } },
{ HYPER|ALT,          XK_Right,   moveresize,        {.v = " 25x   0y   0w   0h" } },
{ HYPER|ALT,          XK_Left,    moveresize,        {.v = "-25x   0y   0w   0h" } },
//Window Size
{ HYPER|ALT|SHIFT,    XK_Down,    moveresize,        {.v = "  0x   0y   0w  25h" } },
{ HYPER|ALT|SHIFT,    XK_Up,      moveresize,        {.v = "  0x   0y   0w -25h" } },
{ HYPER|ALT|SHIFT,    XK_Right,   moveresize,        {.v = "  0x   0y  25w   0h" } },
{ HYPER|ALT|SHIFT,    XK_Left,    moveresize,        {.v = "  0x   0y -25w   0h" } },
  // Volume
{ HYPER,              XK_F1,		  spawn,		         SHCMD("changevolume mute; kill -35 $(pidof dwmblocks)") },
{ HYPER,              XK_F3,  	  spawn,		         SHCMD("changevolume up;   kill -35 $(pidof dwmblocks)") },
{ HYPER,              XK_F2,  	  spawn,		         SHCMD("changevolume down; kill -35 $(pidof dwmblocks)") },
{ 0,                  VOL_MUTE,	  spawn,		         SHCMD("changevolume mute; kill -35 $(pidof dwmblocks)") },
{ 0,                  VOL_UP,	    spawn,	           SHCMD("changevolume up;   kill -35 $(pidof dwmblocks)") },
{ 0,                  VOL_DOWN,	  spawn,		         SHCMD("changevolume down; kill -35 $(pidof dwmblocks)") },
// Screen Brightness
{ 0,                  BRGHT_UP,   spawn,		         SHCMD("xbacklight +10") },
{ 0,                  BRGHT_DOWN, spawn,	  	       SHCMD("xbacklight -10") },
// Custom Keystroke (super + x -> ctrl alt shft + x)
{ SUPER,              XK_a,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"a", NULL } } }, 
{ SUPER,              XK_b,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"b", NULL } } }, 
{ SUPER,              XK_c,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"c", NULL } } }, 
{ SUPER,              XK_d,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"d", NULL } } }, 
{ SUPER,              XK_e,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"e", NULL } } }, 
{ SUPER,              XK_f,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"f", NULL } } }, 
{ SUPER,              XK_g,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"g", NULL } } }, 
{ SUPER,              XK_h,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"h", NULL } } }, 
{ SUPER,              XK_i,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"i", NULL } } }, 
{ SUPER,              XK_j,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"j", NULL } } }, 
{ SUPER,              XK_k,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"k", NULL } } }, 
{ SUPER,              XK_l,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"l", NULL } } }, 
{ SUPER,              XK_m,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"m", NULL } } }, 
{ SUPER,              XK_n,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"n", NULL } } }, 
{ SUPER,              XK_o,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"o", NULL } } }, 
{ SUPER,              XK_p,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"p", NULL } } }, 
{ SUPER,              XK_q,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"q", NULL } } }, 
{ SUPER,              XK_r,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"r", NULL } } }, 
{ SUPER,              XK_s,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"s", NULL } } }, 
{ SUPER,              XK_t,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"t", NULL } } }, 
{ SUPER,              XK_u,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"u", NULL } } }, 
{ SUPER,              XK_v,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"v", NULL } } }, 
{ SUPER,              XK_w,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"w", NULL } } }, 
{ SUPER,              XK_x,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"x", NULL } } }, 
{ SUPER,              XK_y,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"y", NULL } } }, 
{ SUPER,              XK_z,       spawn,             {.v = (const char*[]){ KBD_SCRIPT"z", NULL } } }, 
// Tags | 0 -> all tags
{ HYPER,              XK_0,       view,              {.ui = ~0 } },
{ HYPER|SHIFT,        XK_0,       tag,               {.ui = ~0 } },
TAGKEYS(              XK_1,       0)
TAGKEYS(              XK_2,       1)
TAGKEYS(              XK_3,       2)
TAGKEYS(              XK_4,       3)
TAGKEYS(              XK_5,       4)
TAGKEYS(              XK_6,       5)
TAGKEYS(              XK_7,       6)
TAGKEYS(              XK_8,       7)
TAGKEYS(              XK_9,       8)
};

// Mouse Bindings (for status bar scripts)
static Button buttons[] = {
  /* click            event mask  button        function         argument */
// windows
	{ ClkClientWin,     HYPER,      M_LEFT,       moveorplace,         {.i = 1} },
	{ ClkClientWin,     HYPER,      M_MID,        togglefloating,      {0} },
// resizing 
	{ ClkClientWin,     HYPER,      M_RIGHT,      resizemouse,         {0} },
	{ ClkClientWin,     SUPER,      M_LEFT,       resizemouse,         {0} },
// tag bar
	{ ClkTagBar,        0,          M_LEFT,       view,                {0} },
// clickable status bar
  { ClkStatusText,    0,          M_LEFT,       sigstatusbar,        {.i = 1} },
  { ClkStatusText,    0,          M_MID,        sigstatusbar,        {.i = 2} },
  { ClkStatusText,    0,          M_RIGHT,      sigstatusbar,        {.i = 3} },
  { ClkStatusText,    0,          SCROLL_DOWN,  sigstatusbar,        {.i = 4} },
  { ClkStatusText,    0,          SCROLL_UP,    sigstatusbar,        {.i = 5} },
  { ClkStatusText,    SHIFT,      M_LEFT,       sigstatusbar,        {.i = 6} },
//Tap Window Resize 
  { ClkClientWin,       HYPER,    SCROLL_UP,    resizemousescroll, {.v = &scrollargs[0]} },
  { ClkClientWin,       HYPER,    SCROLL_DOWN,  resizemousescroll, {.v = &scrollargs[1]} },
  { ClkClientWin,       HYPER,    SCROLL_LEFT,  resizemousescroll, {.v = &scrollargs[2]} },
  { ClkClientWin,       HYPER,    SCROLL_RIGHT, resizemousescroll, {.v = &scrollargs[3]} },
};
