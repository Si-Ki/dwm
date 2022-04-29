/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER "firefox"

/* appearance */
static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 10;       /* snap pixel */
static unsigned int gappih    = 4;        /* horiz inner gap between windows */
static unsigned int gappiv    = 4;        /* vert inner gap between windows */
static unsigned int gappoh    = 4;        /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 4;        /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static const int vertpad      = 0;       /* vertical padding of bar */
static const int sidepad      = 0;       /* horizontal padding of bar */
static char *fonts[]          = { "JetBrainsMono-Medium:size=11", "Font Awesome 6 Free:pixelsize=13:antialias=true:autohint=true", "Font Awesome 6 Brands:pixelsize=13:antialias=true:autohint=true", "NotoColorEmoji:pixelsize=12:antialias=true:autohint=true" };
static char normbgcolor[]           = "#272822";
static char normbordercolor[]       = "#ffef00";
static char normfgcolor[]           = "#00ffff";
static char selfgcolor[]            = "#272822";
static char selbordercolor[]        = "#00ffff";
static char selbgcolor[]            = "#00ffff";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spblue", "-g", "100x25", "-e", "bluetoothctl", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spnotes", "-g", "80x20", "-e", "nvim", "-c", "VimwikiIndex", NULL };
const char *spcmd3[] = {TERMINAL, "-n", "spterm", "-g", "110x30", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spblue",      spcmd1},
	{"spnotes",        spcmd2},
	{"spterm",	spcmd3},
};

/* tagging */
/*static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };*/
static const char *tags[] = { "", "", "", "", "", "Extras" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask   switchtotag  isfloating   isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,       	    1 << 5,  	0,		0,           	0,       0,      -1 },
	{ "mpv",      NULL, 	  NULL, 	    1 << 3,  	0,      	0,	     	0,	 0,	 -1 },
	{ "TelegramDesktop", NULL, NULL,	    1 << 2,  	1,      	0,	     	0,	 0,	  1 },
	{ "Zathura", NULL, NULL,	    	    1 << 4,  	1,      	0,	     	0,	 0,	  1 },
	{ TERMCLASS,  NULL,       NULL,       	    0,       	0,      	0,           	1,       0,      -1 },
	{ BROWSER,    NULL,       NULL,     1 << 1,  	1,      	0,           	0,       0,      -1 },
	{ "Nitrogen",	NULL,	NULL,		    0,  	0,      	1,	     	0,	 0,	  1 },
	{ NULL,      "spblue",    NULL,       	    SPTAG(0),	0,      	1,       	1,       0,      -1 },
	{ NULL,      "spnotes",    NULL,       	    SPTAG(1),	0,      	1,       	1,       0,      -1 },
	{ NULL,      "spterm",    NULL,       	    SPTAG(2),	0,      	1,           	1,       0,      -1 },
};

/* layout(s) */
static float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */

	{ "",		spiral },               /* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "[D]",	deck },			/* Master on left, slaves in monocle-like mode on right */
	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },			/* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	XK_j,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	XK_k,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  XK_c,   ACTION##stack,  {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color8",		STRING,	&normbordercolor },
		{ "piiink",		STRING,	&selbordercolor },
		{ "background",		STRING,	&normbgcolor },
		{ "piiink",		STRING,	&normfgcolor },
		{ "foreground",		STRING,	&selfgcolor },
		{ "background",		STRING,	&selbgcolor },
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",		INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",		INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",		FLOAT,	&mfact },
		{ "gappih",		INTEGER, &gappih },
		{ "gappiv",		INTEGER, &gappiv },
		{ "gappoh",		INTEGER, &gappoh },
		{ "gappov",		INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	{ MODKEY|ShiftMask,		XK_h,	spawn,	SHCMD("dmenuhandler") },
	{ MODKEY,			XK_grave,	spawn,	SHCMD("dmenuunicode") },
	/* { MODKEY|ShiftMask,		XK_grave,	togglescratch,	SHCMD("") }, */
	TAGKEYS(			XK_1,		0)
	TAGKEYS(			XK_2,		1)
	TAGKEYS(			XK_3,		2)
	TAGKEYS(			XK_4,		3)
	TAGKEYS(			XK_5,		4)
	TAGKEYS(			XK_6,		5)
	{ MODKEY,			XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,		XK_0,		tag,		{.ui = ~0 } },
	{ MODKEY,			XK_minus,	spawn,		SHCMD("pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,		XK_minus,	spawn,		SHCMD("pamixer --allow-boost -d 15; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			XK_equal,	spawn,		SHCMD("pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)") },
	{ MODKEY|ShiftMask,		XK_equal,	spawn,		SHCMD("pamixer --allow-boost -i 15; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			XK_BackSpace,	spawn,		SHCMD("sysact") },
	{ MODKEY|ShiftMask,		XK_BackSpace,	spawn,		SHCMD("sysact") },

	{ MODKEY,			XK_Tab,		view,		{0} },
	{ MODKEY|ShiftMask,		XK_s,		spawn,		SHCMD("maim -u -s -f png -m 10 | xclip -sel c -t image/png") },
	{ MODKEY,			XK_q,		killclient,	{0} },
	{ MODKEY|ShiftMask,		XK_q,		spawn,		SHCMD("telegram-desktop") },
	{ MODKEY,			XK_w,		spawn,		SHCMD(BROWSER) },
	{ MODKEY|ShiftMask,		XK_w,		spawn,		SHCMD(TERMINAL " -e sudo nmtui") },
	{ MODKEY,			XK_e,		spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks; rmdir ~/.abook") },
	{ MODKEY|ShiftMask,		XK_e,		spawn,		SHCMD(TERMINAL " -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") },
	/*{ MODKEY,			XK_r,		spawn,		SHCMD("thunar") },*/
	{ ControlMask,			XK_Escape,	spawn,		SHCMD(TERMINAL " -e htop") },
	{ MODKEY,			XK_t,		setlayout,	{.v = &layouts[0]} }, /* tile */
	{ MODKEY|ShiftMask,		XK_t,		setlayout,	{.v = &layouts[1]} }, /* bstack */
	{ MODKEY,			XK_y,		setlayout,	{.v = &layouts[2]} }, /* spiral */
	{ MODKEY|ShiftMask,		XK_y,		setlayout,	{.v = &layouts[3]} }, /* dwindle */
	{ MODKEY,			XK_u,		setlayout,	{.v = &layouts[4]} }, /* deck */
	{ MODKEY|ShiftMask,		XK_u,		setlayout,	{.v = &layouts[5]} }, /* monocle */
	{ MODKEY,			XK_i,		setlayout,	{.v = &layouts[6]} }, /* centeredmaster */
	{ MODKEY|ShiftMask,		XK_i,		setlayout,	{.v = &layouts[7]} }, /* centeredfloatingmaster */
	{ MODKEY,			XK_o,		incnmaster,     {.i = +1 } },
	{ MODKEY|ShiftMask,		XK_o,		incnmaster,     {.i = -1 } },
	{ MODKEY,			XK_p,			spawn,		SHCMD("playerctl play-pause") },
	/*{ MODKEY|ShiftMask,		XK_p,			spawn,		SHCMD("playerctl  ; pauseallmpv") }, */
//	{ MODKEY,			XK_bracketleft,		spawn,		SHCMD("playerctl position -10") },
//	{ MODKEY|ShiftMask,		XK_bracketleft,		spawn,		SHCMD("playerctl position -60") },
//	{ MODKEY,			XK_bracketright,	spawn,		SHCMD("playerctl position +10") },
//	{ MODKEY|ShiftMask,		XK_bracketright,	spawn,		SHCMD("playerctl position +60") },
	{ MODKEY,			XK_backslash,		view,		{0} },
	/* { MODKEY|ShiftMask,		XK_backslash,		spawn,		SHCMD("") }, */

	{ MODKEY,			XK_a,		togglegaps,	{0} },
	{ MODKEY|ShiftMask,		XK_a,		defaultgaps,	{0} },
	{ MODKEY,			XK_s,		togglesticky,	{0} },
	/* { MODKEY|ShiftMask,		XK_s,		spawn,		SHCMD("") }, */
	{ Mod1Mask,			XK_space,	spawn,          SHCMD("dmenu_run -i") },
	{ MODKEY,			XK_v,		spawn,          SHCMD("clipmenu -l 10") },
	{ MODKEY,			XK_d,		spawn,		SHCMD("firefox https://discord.com/channels/@me") },
	{ MODKEY|ShiftMask,		XK_d,		spawn,		SHCMD("passmenu --type") },
	{ MODKEY,			XK_f,		togglefullscr,	{0} },
	{ MODKEY|ShiftMask,		XK_f,		setlayout,	{.v = &layouts[8]} },
	{ MODKEY,			XK_Left,		shiftview,	{ .i = -1 } },
	{ MODKEY|ShiftMask,		XK_Left,		shifttag,	{ .i = -1 } },
	{ MODKEY,			XK_h,		setmfact,	{.f = -0.05} },
	/* J and K are automatically bound above in STACKEYS */
	{ MODKEY,			XK_l,		setmfact,      	{.f = +0.05} },
	{ MODKEY,			XK_Right,	shiftview,	{ .i = 1 } },
	{ MODKEY|ShiftMask,		XK_Right,	shifttag,	{ .i = 1 } },
	{ MODKEY,			XK_apostrophe,	togglescratch,	{.ui = 0} },
	{ MODKEY|ShiftMask,		XK_Return,	togglescratch,	{.ui = 2} },
	/* { MODKEY|ShiftMask,		XK_apostrophe,	spawn,		SHCMD("") }, */
	{ MODKEY,			XK_Return,	spawn,		{.v = termcmd } },
	{ MODKEY,			XK_r,		togglescratch,	{.ui = 1} },

	{ MODKEY,			XK_z,		incrgaps,	{.i = +3 } },
	{ MODKEY|ShiftMask,		XK_z,		spawn,		SHCMD("share") },
	{ MODKEY,			XK_x,		incrgaps,	{.i = -3 } },
	/* { MODKEY|ShiftMask,		XK_x,		spawn,		SHCMD("") }, */
	/* { MODKEY,			XK_c,		spawn,		SHCMD("") }, */
	/* { MODKEY|ShiftMask,		XK_c,		spawn,		SHCMD("") }, */
	/* V is automatically bound above in STACKKEYS */
	{ MODKEY,			XK_b,		togglebar,	{0} },
	{ MODKEY|ShiftMask,		XK_b,		spawn,		SHCMD("bookread") },
	{ MODKEY|ShiftMask,		XK_m,		spawn,		SHCMD(TERMINAL " -e nvim -c VimwikiIndex") },
	{ MODKEY|ShiftMask,		XK_n,		spawn,		SHCMD(TERMINAL " -e newsboat; pkill -RTMIN+6 dwmblocks") },
	{ MODKEY,			XK_n,		spawn,		SHCMD("notflix") },
	{ MODKEY,			XK_m,		spawn,		SHCMD("muteunmute") },
	/*{ MODKEY|ShiftMask,		XK_n,		spawn,		SHCMD("muteunmute") }, */
	{ MODKEY,			XK_comma,	spawn,		SHCMD("playerctl previous") },
	{ MODKEY|ShiftMask,		XK_comma,	spawn,		SHCMD("playerctl positon 0%") },
	{ MODKEY,			XK_period,	spawn,		SHCMD("playerctl next") },
	{ MODKEY|ShiftMask,		XK_period,	spawn,		SHCMD("playerctl repeat") },

	{ MODKEY,			XK_bracketleft,	focusmon,	{.i = -1 } },
	{ MODKEY|ShiftMask,		XK_bracketleft,	tagmon,		{.i = -1 } },
	{ MODKEY,			XK_bracketright, focusmon,	{.i = +1 } },
	{ MODKEY|ShiftMask,		XK_bracketright, tagmon,	{.i = +1 } },

	{ MODKEY,			XK_Page_Up,	shiftview,	{ .i = -1 } },
	{ MODKEY|ShiftMask,		XK_Page_Up,	shifttag,	{ .i = -1 } },
	{ MODKEY,			XK_Page_Down,	shiftview,	{ .i = +1 } },
	{ MODKEY|ShiftMask,		XK_Page_Down,	shifttag,	{ .i = +1 } },
	{ MODKEY,			XK_Insert,	spawn,		SHCMD("xdotool type $(grep -v '^#' ~/.local/share/larbs/snippets | dmenu -i -l 50 | cut -d' ' -f1)") },

	{ MODKEY,			XK_F1,		spawn,		SHCMD("xset dmps force off") },
	{ MODKEY,			XK_F2,		spawn,		SHCMD("tutorialvids") },
	{ MODKEY,			XK_F3,		spawn,		SHCMD("displayselect") },
	{ MODKEY,			XK_F4,		spawn,		SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") },
	/* { MODKEY,			XK_F5,		xrdb,		{.v = NULL } }, */
	{ MODKEY,			XK_F6,		spawn,		SHCMD("torwrap") },
	{ MODKEY,			XK_F7,		spawn,		SHCMD("td-toggle") },
	{ MODKEY,			XK_F8,		spawn,		SHCMD("mw -Y") },
	{ MODKEY,			XK_F9,		spawn,		SHCMD("dmenumount") },
	{ MODKEY,			XK_F10,		spawn,		SHCMD("dmenuumount") },
	{ MODKEY,			XK_F11,		spawn,		SHCMD("mpv --vf=hflip,rotate=0 --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam /dev/video*") },
	{ MODKEY,			XK_F12,		spawn,		SHCMD("checkup") },
	{ MODKEY,			XK_space,	zoom,		{0} },
	{ MODKEY|ShiftMask,		XK_space,	togglefloating,	{0} },

/*	{ ShiftMask,			XK_Print,	spawn,		SHCMD("miampick") }, */
	{ MODKEY,			XK_Print,	spawn,		SHCMD("dmenurecord") },
	{ MODKEY|ShiftMask,		XK_x,		spawn,		SHCMD("maimpick") },
	{ MODKEY,			XK_Delete,	spawn,		SHCMD("dmenurecord kill") },
	{ MODKEY,			XK_Scroll_Lock,	spawn,		SHCMD("killall screenkey || screenkey -f JetBrainsMono &") },

	{ 0, XF86XK_AudioMute,		spawn,		SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("pamixer --allow-boost -i 3; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("pamixer --allow-boost -d 3; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPrev,		spawn,		SHCMD("playerctl previous") },
	{ 0, XF86XK_AudioNext,		spawn,		SHCMD("playerctl next") },
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("playerctl play-pause") },
	{ 0, XF86XK_AudioMedia,		spawn,		SHCMD(TERMINAL " -e ncplayerctlpp") },
/*	{ 0, XF86XK_AudioMicMute,	spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") }, */
	{ 0, XF86XK_PowerOff,		spawn,		SHCMD("sysact") },
	{ 0, XF86XK_Calculator,		spawn,		SHCMD(TERMINAL " -e bc -l") },
	{ 0, XF86XK_Sleep,		spawn,		SHCMD("sudo -A zzz") },
	{ 0, XF86XK_WWW,		spawn,		SHCMD("Brave-browser") },
	{ 0, XF86XK_DOS,		spawn,		SHCMD(TERMINAL) },
	{ MODKEY,	XK_Escape,	spawn,		SHCMD("gpg-connect-agent --no-autostart reloadagent /bye && slock & xset dpms force off;") },
	{ 0, XF86XK_TaskPane,		spawn,		SHCMD(TERMINAL " -e htop") },
	{ 0, XF86XK_Mail,		spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	{ 0, XF86XK_MyComputer,		spawn,		SHCMD(TERMINAL " -e lf /") },
	/* { 0, XF86XK_Battery,		spawn,		SHCMD("") }, */
	{ 0, XF86XK_Launch1,		spawn,		SHCMD("xset dpms force off") },
	{ 0, XF86XK_TouchpadToggle,	spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOff,	spawn,		SHCMD("synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOn,		spawn,		SHCMD("synclient TouchpadOff=0") },
	{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("xbacklight -inc 15") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("xbacklight -dec 15") },

	/* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
	/* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
	/* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
	/* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,		MODKEY,		Button4,	incrgaps,	{.i = +1} },
	{ ClkClientWin,		MODKEY,		Button5,	incrgaps,	{.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		Button4,	shiftview,	{.i = -1} },
	{ ClkTagBar,		0,		Button5,	shiftview,	{.i = 1} },
	{ ClkRootWin,		0,		Button2,	togglebar,	{0} },
};

