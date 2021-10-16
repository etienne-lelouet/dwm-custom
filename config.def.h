
#define DEBUG 0
#define DEBUGPATH "/tmp/logfile"

#include <X11/XF86keysym.h> // pour avoir la correspondance multemedia keys xf86 keys
/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class	instance	title		tags mask	isfloating	monitor */
	{ "Steam",	"Steam",	"Steam",	0,		1,		-1 },
	{ "qemu",	"qemu",		"QEMU",		0,		1,		-1 }
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      comboview,		{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     	{.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      combotag,			{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      	{.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

static const char *mutecmd[] = { "pactl-volumectl", "m", "--displaynotification", NULL };
static const char *volupcmd[] = { "pactl-volumectl", "+", "5", "--displaynotification", NULL };
static const char *voldowncmd[] = { "pactl-volumectl", "-", "5", "--displaynotification",  NULL };

static const char *brupcmd[] = { "brightnessctl", "-d", "intel_backlight", "set", "5%+", NULL };
static const char *brdowncmd[] = { "brightnessctl", "-d", "intel_backlight", "set", "5%-", NULL };

static const char *printroot[] = {
	"dwmcustom-screenshot", "root", NULL
};

static const char *printactive[] = {
	"dwmcustom-screenshot", "active", NULL
};

static const char *printselection[] = {
	"dwmcustom-screenshot", NULL
};

static const char *pavucontrol[] = {
	"pavucontrol-qt", NULL
};

static const char *lockcmd[] = { "slock", NULL };

static Key keys[] = {
	/* modifier                     key        			function        argument */
	{ MODKEY,                       XK_p,      					spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, 					spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      					togglebar,      {0} },
	{ MODKEY,                       XK_j,      					focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      					focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Up,      				focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Down,					focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      					incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      					incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      					setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      					setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, 					zoom,           {0} },
	{ MODKEY,                       XK_Tab,    					view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      					killclient,     {0} },
	{ MODKEY,                       XK_t,      					setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      					setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      					setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  					setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  					togglefloating, {0} },
	{ MODKEY,                       XK_0,      					view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      					tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_w,						focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_x,						focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_w,						tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_x,						tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,				XK_l,						spawn, 			{.v = lockcmd } },
	{ MODKEY|ShiftMask,				XK_v,						spawn,			{.v = pavucontrol } },
	{ MODKEY|ControlMask,			XF86XK_Sleep				spawn,			{.v = autorandr-c } }
	{ 0,							XF86XK_AudioMute,			spawn, 			{.v = mutecmd } },
	{ 0,							XF86XK_AudioLowerVolume,	spawn, 			{.v = voldowncmd } },
	{ 0,							XF86XK_AudioRaiseVolume,	spawn, 			{.v = volupcmd } },
	// because fn is now meta
	{ MODKEY,						XF86XK_AudioMute,			spawn, 			{.v = mutecmd } },
	{ MODKEY,						XF86XK_AudioLowerVolume,	spawn, 			{.v = voldowncmd } },
	{ MODKEY,						XF86XK_AudioRaiseVolume,	spawn, 			{.v = volupcmd } },
	{ 0,							XF86XK_MonBrightnessUp,		spawn, 			{.v = brupcmd} },
	{ 0,							XF86XK_MonBrightnessDown,	spawn, 			{.v = brdowncmd} },
	{ 0,							XK_Print,					spawn,			{.v = printactive} },
	{ MODKEY,						XK_Print,					spawn,			{.v = printroot} },
	{ ShiftMask,					XK_Print,					spawn,			{.v = printselection} },
	{ MODKEY,                       XK_Left,      				comboviewshift,	{.i = -1} },
	{ MODKEY,                       XK_Right,      				comboviewshift,	{.i = +1} },
	// TAGKEYS ON QWERTY
	TAGKEYS(                        XK_1,                      	0)
	TAGKEYS(                        XK_2,                      	1)
	TAGKEYS(                        XK_3,                      	2)
	TAGKEYS(                        XK_4,                      	3)
	TAGKEYS(                        XK_5,                      	4)
	TAGKEYS(                        XK_6,                      	5)
	TAGKEYS(                        XK_7,                      	6)
	TAGKEYS(                        XK_8,                      	7)
	TAGKEYS(                        XK_9,                      	8)
	// TAGKEYS ON AZERTY
	TAGKEYS(                        0x26,                      	0)
	TAGKEYS(                        0xe9,                      	1)
	TAGKEYS(                        0x22,                      	2)
	TAGKEYS(                        0x27,                      	3)
	TAGKEYS(                        0x28,                      	4)
	TAGKEYS(                        0x2d,                      	5)
	TAGKEYS(                        0xe8,                      	6)
	TAGKEYS(                        0x5f,                      	7)
	TAGKEYS(                        0xe7,                      	8)
	{ MODKEY|ShiftMask,             XK_q,      					quit,           	{0} }
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

