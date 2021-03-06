/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 1;        /* border pixel of windows */
static unsigned int gappx     = 5;        /* gaps between windows */
static unsigned int snap      = 32;       /* snap pixel */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */

// static const int usealtbar          = 1;        /* 1 means use non-dwm status bar */
// static const char *altbarclass      = "Polybar"; /* Alternate bar class name */
// static const char *alttrayname      = "tray";    /* Polybar tray instance name */
// static const char *altbarcmd        = "$HOME/scripts/bar.sh"; /* Alternate bar launch command */

static char font[]            = "monospace:size=10";
static const char *fonts[]    = { font, "FontAwesome:size=16" };
static const char dmenufont[] = "monospace:size=10";

static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[]     = "#bbbbbb";
static char selfgcolor[]      = "#eeeeee";
static char selbordercolor[]  = "#005577";
static char selbgcolor[]      = "#005577";
static char *colors[][3]        = {
	/*               fg         bg         border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};


// #if OPAQUE = OPAQUE
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalpha = OPAQUE;
static unsigned int alphas[][3] = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};
// #endif

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance    title       tags mask     isfloating   monitor */
	{ "Gimp",               NULL,       NULL,       0,            1,           -1 },
	{ "brave-browser",      NULL,       NULL,       1 << 8,       0,           -1 },
	{ "stalonetray",        NULL,       NULL,       1 << 8,       1,           -1 },

	{ "usbguard-applet-qt", NULL, "USB Device Inserted", 0,       1,           -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

#define VOLCTRL(amt)   { .v = (const char*[]){ "/usr/bin/pactl", "set-sink-volume", "0", amt, NULL } }
#define AUDIOCTRL(cmd) { .v = (const char*[]){ "/usr/bin/playerctl", cmd, NULL } }
#define BRGCTRL(amt)   { .v = (const char*[]){ "/usr/bin/brightnessctl", "set", amt, NULL } }

static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "0", "toggle",  NULL };


/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
	{ "font",               STRING,  &font },

	// { "color0",             STRING,  &normbgcolor },
	// { "color8",             STRING,  &normbordercolor },
	// { "color15",            STRING,  &normfgcolor },
	// { "color2",             STRING,  &selbgcolor },
	// { "color15",            STRING,  &selbordercolor },
	// { "color15",            STRING,  &selfgcolor },	

	{ "color0",		STRING,	&normbordercolor },
	{ "color8",		STRING,	&selbordercolor },
	{ "color0",		STRING,	&normbgcolor },
	{ "color4",		STRING,	&normfgcolor },
	{ "color0",		STRING,	&selfgcolor },
	{ "color4",		STRING,	&selbgcolor },

	// { "normbgcolor",        STRING,  &normbgcolor },
	// { "normbordercolor",    STRING,  &normbordercolor },
	// { "normfgcolor",        STRING,  &normfgcolor },
	// { "selbgcolor",         STRING,  &selbgcolor },
	// { "selbordercolor",     STRING,  &selbordercolor },
	// { "selfgcolor",         STRING,  &selfgcolor },

	{ "borderpx",          	INTEGER, &borderpx },
	{ "gappx",          	INTEGER, &gappx },
	{ "snap",          		INTEGER, &snap },
	{ "showbar",          	INTEGER, &showbar },
	{ "topbar",          	INTEGER, &topbar },
	{ "nmaster",          	INTEGER, &nmaster },
	{ "resizehints",       	INTEGER, &resizehints },
	{ "mfact",      	 	FLOAT,   &mfact },
};

#include "selfrestart.c"
#include "shiftview.c"

#include <X11/XF86keysym.h>

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_grave,  togglescratch,  {.v = scratchpadcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	// { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	// { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY|ShiftMask,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_x,      transfer,       {0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = "0x 0y -25w 0h" } },	
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	{ MODKEY,                       XK_w,      shiftview,      {.i = +1 } },
	{ MODKEY,                       XK_q,      shiftview,      {.i = -1 } },	
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_r,      self_restart,   {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

	/* MEDIA */
	{ 0,                XF86XK_AudioMute,      spawn,          {.v = mutevol } },
	{ 0,         XF86XK_AudioLowerVolume,      spawn,          VOLCTRL("-5%") },
	{ 0,         XF86XK_AudioRaiseVolume,      spawn,          VOLCTRL("+5%") },
	{ ShiftMask, XF86XK_AudioLowerVolume,      spawn,          VOLCTRL("-1%") },
	{ ShiftMask, XF86XK_AudioRaiseVolume,      spawn,          VOLCTRL("+1%") },
	{ MODKEY|ShiftMask,            XK_F1,      spawn,          AUDIOCTRL("previous") },
	{ MODKEY|ShiftMask,            XK_F2,      spawn,          AUDIOCTRL("next") },
	// { MODKEY,                       XK_Escape, spawn,          AUDIOCTRL("play-pause") },
	{ MODKEY,                      XK_Escape,  spawn,          SHCMD("~/scripts/media-toggle.sh") },
	/* FN */
	{ MODKEY,                       XK_l,      spawn,          SHCMD("xautolock -locknow") },
	{ 0,         XF86XK_MonBrightnessDown,     spawn,          BRGCTRL("10%-") },
	{ ShiftMask, XF86XK_MonBrightnessDown,     spawn,          BRGCTRL("5%-") },
	{ 0,         XF86XK_MonBrightnessUp,       spawn,          BRGCTRL("+10%") },
	{ ShiftMask, XF86XK_MonBrightnessUp,       spawn,          BRGCTRL("+5%") },
	{ 0,         XF86XK_Calculator,            spawn,          SHCMD("$CALCULATOR") },

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

#ifdef IPCCOMMAND
static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};
#endif