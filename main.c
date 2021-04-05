// PART Of PSP Module Checker 4.0 SRC
// By West-Li
// main.c - main file of the app

#include <pspkernel.h>  
#include <pspdebug.h> 
#include <pspctrl.h>
#include <psppower.h>
#include <stdio.h>

#include <kubridge.h>

#include "modules/ipl_update.h"
#include "modules/batman.h"
#include "modules/kuman.h"

#include "menu.h"
#include "kuman_header.h"
#include "systemctrl_se.h"

#define Version "flash0:/vsh/etc/version.txt"

PSP_MODULE_INFO("Checker", 0x200, 1, 0); //0x200 flag we need for use psp-packer

extern void CheckerPrintf(char *fmt, ...);

int color = 0;
u32 value;

SceCtrlData pad;

SceUID fd;

SEConfig config;

char BatSerCurrent[4];

void ShowPage5()
{
    int type, size, psp, pan, cfw;
	
	CheckerInitMainScreen("Page 5 - Advanced Configuration, CFW & PSP Utils Info");
	
	psp = chGetPSPHackable();
	pan = chGetPSPCreatePandora();
    cfw = chDetectCFW();
	
	chGetIplAttribute(&type, &size);
	sctrlSEGetConfig(&config);
	
	CheckerPrintf("Plain modules in UMD/ISO     (currently: %s)\n", config.umdactivatedplaincheck ? "Enabled" : "Disabled");
    CheckerPrintf("Execute PBOOT.BIN in UMD/ISO (currently: %s)\n", config.executebootbin ? "Enabled" : "Disabled");
    CheckerPrintf("XMB plugins                  (currently: %s)\n", config.xmbplugins ? "Disabled" : "Enabled");
    CheckerPrintf("GAME plugins                 (currently: %s)\n", config.gameplugins ? "Disabled" : "Enabled");
    CheckerPrintf("POPS plugins                 (currently: %s)\n\n", config.popsplugins ? "Disabled" : "Enabled");
    CheckerPrintf("IPL:                %s (Size: 0x%08X)\n", IPLs[type], size);
	CheckerPrintf("CFW:                %s\n\n", CFWs[cfw]);
	CheckerPrintf("PSP Hackable:       %s\n", PSPUtils[psp]);
	CheckerPrintf("PSP Create Pandora: %s\n\n\n", PSPUtils[pan]);
	
	CheckerPrintf("Press < to switch page, R to Edit, X to Back, O to Exit...\n");
	
	sceKernelDelayThread(200000);
	
	while (1)
	{
		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_LEFT)
		{
			sceKernelDelayThread(250000);
			ShowPage4();
        }
		if (pad.Buttons & PSP_CTRL_RTRIGGER)
		{
			ShowAdvancedCnfMenu();
        }
		if (pad.Buttons & PSP_CTRL_CROSS)
		{
		    ShowMainMenu();
		}
		if (pad.Buttons & PSP_CTRL_CIRCLE)
		{
			CheckerExit();
        }
		
		sceKernelDelayThread(10000);
	}
}

void ShowPage4()
{
    CheckerInitMainScreen("Page 4 - Configurations Info");

	sctrlSEGetConfig(&config);
	
	CheckerPrintf("Skip Sony logo                 (currently: %s)\n", config.skiplogo ? "Enabled" : "Disabled");
    CheckerPrintf("Hide corrupt icons             (currently: %s)\n", config.hidecorrupt ? "Enabled" : "Disabled");
    CheckerPrintf("Game folder homebrew           (currently: %s)\n", config.gamekernel150 ? "1.50 Kernel" : "6.XX Kernel");
    CheckerPrintf("Autoboot program at /PSP/GAME/BOOT/ (currently: %s)\n", config.startupprog ? "Enabled" : "Disabled");
    CheckerPrintf("UMD Mode                       (currently: %s)\n", umdmodes[config.umdmode]);
    CheckerPrintf("Fake region                    (currently: %s)\n", regions[config.fakeregion]);
    CheckerPrintf("Use VshMenu                    (currently: %s)\n", vshmenuoptions[config.vshmenu]);
    CheckerPrintf("XMB Usb Device                 (currently: %s)\n", usbdevices[config.usbdevice]);
    CheckerPrintf("Use network update             (currently: %s)\n", config.notusedaxupd ? "Disabled" : "Enabled");
    CheckerPrintf("Hide PIC1.PNG and PIC0.PNG     (currently: %s)\n", config.hidepics ? "Enabled" : "Disabled");
    CheckerPrintf("Use version.txt                (currently: %s)\n", config.useversiontxt ? "Enabled" : "Disabled");
    CheckerPrintf("Use Slim colors on Classic PSP (currently: %s)\n", config.slimcolors ? "Enabled" : "Disabled");
    CheckerPrintf("Hide MAC address               (currently: %s)\n\n\n", config.hidemac ? "Enabled" : "Disabled");
	
	CheckerPrintf("Press < & > to switch page, R to Edit, X to Back, O to Exit...\n");
	
	sceKernelDelayThread(200000);
	
	while (1)
	{
		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_LEFT)
		{
			sceKernelDelayThread(250000);
			ShowPage3();
        }
		if (pad.Buttons & PSP_CTRL_RIGHT)
		{
			sceKernelDelayThread(250000);
			ShowPage5();
        }
		if (pad.Buttons & PSP_CTRL_RTRIGGER)
		{
			ShowCnfMenu();
        }
		if (pad.Buttons & PSP_CTRL_CROSS)
		{
		    ShowMainMenu();
		}
		if (pad.Buttons & PSP_CTRL_CIRCLE)
		{
			CheckerExit();
        }
		
		sceKernelDelayThread(10000);
	}
}

//Based on raing3s CheckSysInfoP4 
void ShowPage3()
{
    int cpu, bus;
	char name[32], pass[5], button[256], wma[256], flash[256];
	u8 kirk[4], spock[4], macaddr[512];
	
	*(u32 *)kirk = chGetKirk();
	*(u32 *)spock = chGetSpock();
	
	GetRegistryValue("/CONFIG/SYSTEM/XMB", "button_assign", &value);

    if (value == 1) 
    {
        sprintf(button, "Swap buttons: Yes (currently: O is enter)\n");		
    } 
	else 
	{
        sprintf(button, "Swap buttons: No (currently: X is enter)\n");		
    }
	
	GetRegistryValue("/CONFIG/MUSIC", "wma_play", &value);

    if (value == 1)
	{
		sprintf(wma, "WMA:          Yes\n");	
	}
	else
	{
	    sprintf(wma, "WMA:          No\n");		
	}
	
	GetRegistryValue("/CONFIG/BROWSER", "flash_activated", &value);

    if (value == 1)
	{
		sprintf(flash, "Flash player: Yes\n\n\n");	
	}
	else
	{
	    sprintf(flash, "Flash player: No\n\n\n");		
	}

	memset(name, 0, sizeof(name));
	memset(pass, 0, sizeof(pass));
	
	chGetCpuSpeed(&cpu, &bus);
	chGetMACAddress(macaddr);
	
	CheckerInitMainScreen("Page 3 - System Info");

	CheckerPrintf("CPU Speed:    %i/%i Mhz\n", cpu, bus);
	CheckerPrintf("WLAN:         %s\n\n", sceWlanGetSwitchState() == 0 ? "Off" : "On");
	CheckerPrintf("Name:         %s\n", GetRegistryValue("/CONFIG/SYSTEM", "owner_name", &name, sizeof(name)));
	CheckerPrintf("Password:     %s\n", GetRegistryValue("/CONFIG/SYSTEM/LOCK", "password", &pass, sizeof(pass)));
	
	if(VerifyFile("machide.chk") == -1)
	   CheckerPrintf("MAC Address:  %02X:%02X:%02X:%02X:%02X:%02X\n\n", macaddr[0], macaddr[1], macaddr[2], macaddr[3], macaddr[4], macaddr[5]);
	else
	   CheckerPrintf("MAC Address:  00:00:00:00:00:00 (Hidden)\n\n");   
	   
	CheckerPrintf("Scramble:     0x%08X\n\n", chGetScramble());
	CheckerPrintf("Kirk:         %c%c%c%c\n", kirk[3], kirk[2], kirk[1], kirk[0]);
	
	if(kuKernelGetModel() == 4) CheckerPrintf("Spock:        -\n\n");
	else CheckerPrintf("Spock:        %c%c%c%c\n\n", spock[3], spock[2], spock[1], spock[0]);  
	
	CheckerPrintf(button);
	CheckerPrintf(wma);
	CheckerPrintf(flash);
	
	CheckerPrintf("Press < & > to switch page, R to Edit, X to Back, O to Exit...\n");
	
	sceKernelDelayThread(200000);
	
	while (1)
	{
		sceCtrlReadBufferPositive(&pad, 1);
		
		if (pad.Buttons & PSP_CTRL_LEFT)
		{
			sceKernelDelayThread(250000);
			ShowPage2();
        }
		if (pad.Buttons & PSP_CTRL_RIGHT)
		{
			sceKernelDelayThread(250000);
			ShowPage4();
        }
		if (pad.Buttons & PSP_CTRL_RTRIGGER)
		{
			ShowSystemMenu();
        }
		if (pad.Buttons & PSP_CTRL_CROSS)
		{
		    ShowMainMenu();
		}
		if (pad.Buttons & PSP_CTRL_CIRCLE)
		{
			CheckerExit();
        }
		
		sceKernelDelayThread(10000);
	}
}

//Based on raing3s CheckSysInfoP2
void ShowPage2()
{
    int batser, batmode;
	
	CheckerInitMainScreen("Page 2 - Battery Info");
	
	batmode = GetBatType();
	batser = GetBatSer();

	CheckerPrintf("Battery Status:  %s\n", scePowerIsBatteryCharging() == 1 ? "Charging" : "Using");
	pspDebugScreenPrintf(" Battery %%:       %i%%\n\n", scePowerGetBatteryLifePercent() < 0 ? 0 : scePowerGetBatteryLifePercent()); //Here i use printf bcz my CheckerPrintf dont printf % (ill fix it later)
	CheckerPrintf("Hours to Left:   %i:%02i\n\n", scePowerGetBatteryLifeTime() < 0 ? 0 : (scePowerGetBatteryLifeTime() / 60), scePowerGetBatteryLifeTime() < 0 ? 0 : (scePowerGetBatteryLifeTime() - (scePowerGetBatteryLifeTime() / 60 * 60)));
	CheckerPrintf("Battery Temp:    %i�C\n", scePowerGetBatteryTemp() < 0 ? 0 : scePowerGetBatteryTemp());
	CheckerPrintf("Battery Voltage: %0.3fV\n\n", scePowerGetBatteryVolt() < 0 ? 0 : (float)scePowerGetBatteryVolt() / 1000.0);
	CheckerPrintf("Remain Capacity: %i mAh\n", scePowerGetBatteryRemainCapacity() < 0 ? 0 : scePowerGetBatteryRemainCapacity()); //From raing3s psppower
	CheckerPrintf("Total Capacity:  %i mAh\n\n", scePowerGetBatteryFullCapacity() < 0 ? 0 : scePowerGetBatteryFullCapacity()); //From raing3s psppower
	
	if(chGetPSPCreatePandora() == 0) //Yes so we can show the serial, mode of battery
	{
	    CheckerPrintf("Battery Type:    %s\n", Batterys[batmode]);
	    CheckerPrintf("Battery Serial:  0x%08X\n\n\n", batser);
	}
	else
	{
	    CheckerPrintf("Battery Type:    -\n");
	    CheckerPrintf("Battery Serial:  -\n\n\n");
	}
	
	CheckerPrintf("Press < & > to switch pages, R to Edit, X to Back, O to Exit...\n");
	
	sceKernelDelayThread(200000);
	
	while (1)
	{
		sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_LEFT)
		{
			sceKernelDelayThread(250000);
			ShowPage1();
        }
		if (pad.Buttons & PSP_CTRL_RIGHT)
		{
			sceKernelDelayThread(250000);
			ShowPage3();
        }
		if (pad.Buttons & PSP_CTRL_RTRIGGER)
		{
			ShowBatteryMenu();
        }
		if (pad.Buttons & PSP_CTRL_CROSS)
		{
		    ShowMainMenu();
		}
		if (pad.Buttons & PSP_CTRL_CIRCLE)
		{
			CheckerExit();
        }
		
		sceKernelDelayThread(10000);
	}
}

void ShowPage1()
{
    int baryon, pommel, tachyon, fuseid, fusecfg, mb, model, type, region;
	char *unk_minor = "-";
	
	CheckerInitMainScreen("Page 1 - PSP Info");
	
	chGetVersions(&baryon, &pommel, &tachyon, &fuseid, &fusecfg);
	
	mb = chDetectMotherboard();
    model = chDetectModel();	
    type = chDetectOFW();
    region = chGetRegion();		

	CheckerPrintf("Kernel Version: %s (0x%08X)\n\n", FWs[type], sceKernelDevkitVersion());
	
	if(region == 10)
	   CheckerPrintf("Model:          %s%i\n", PSPs[model], unk_minor);
	else if(model == 4)
	   CheckerPrintf("Model:          %s\n", PSPs[model]);   
	else
	   CheckerPrintf("Model:          %s%i\n", PSPs[model], region);
	   
	CheckerPrintf("Module:         %s\n", Modules[model]);      
	CheckerPrintf("Motherboard:    %s\n\n", MBs[mb]);
	CheckerPrintf("Tachyon:        0x%08X\n", tachyon);
	CheckerPrintf("Baryon:         0x%08X\n", baryon);
	CheckerPrintf("Pommel:         0x%08X\n\n", pommel);
	CheckerPrintf("Fuse ID:        0x%04X%08X\n", fuseid);
	CheckerPrintf("Fuse CFG:       0x%08X\n\n", fusecfg);
	CheckerPrintf("Region:         %s\n\n\n", Regions[region]);
	
	CheckerPrintf("Press > to switch page, X to Back, O to Exit...\n");
	
	sceKernelDelayThread(200000);
	
	while (1)
	{
		sceCtrlReadBufferPositive(&pad, 1);
		
		if (pad.Buttons & PSP_CTRL_RIGHT)
		{
			sceKernelDelayThread(250000);
			ShowPage2();
        }
		if (pad.Buttons & PSP_CTRL_CROSS)
		{
		    ShowMainMenu();
		}
		if (pad.Buttons & PSP_CTRL_CIRCLE)
		{
			CheckerExit();
        }
		
		sceKernelDelayThread(10000);
	}
}

void ShowVersionTxt()
{
	CheckerInitMainScreen("Version.txt - Show");
	
	CheckerPrintf("Checking...");
	
	if(CheckVersion(Version) == -1)
	   Error(" Version.txt not found...");
	   
	sceKernelDelayThread(2000000);      
	
    CheckerPrintf("OK\n\n\n");
	CheckerPrintf("Version.txt:");
    CheckerPrintf("\n%s\n\n", GetVersion());	
	
	CheckerPrintf("Press X to Back to MainMenu, O to Exit...\n\n");
	
	while(1)
	{
	    sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_CROSS)
		{
		    ShowMainMenu();
		}
		if (pad.Buttons & PSP_CTRL_CIRCLE)
		{
		    CheckerExit();
		}
	}
}

void ShowAdvancedCnfMenu(void)
{
    int btn;
	
	sctrlSEGetConfig(&config);

	sprintf(patch, "Plain modules in UMD/ISO     (currently: %s)", config.umdactivatedplaincheck ? "Enabled" : "Disabled");
    sprintf(bootbin, "Execute PBOOT.BIN in UMD/ISO (currently: %s)", config.executebootbin ? "Enabled" : "Disabled");
    sprintf(xmb, "XMB plugins                  (currently: %s)", config.xmbplugins ? "Disabled" : "Enabled");
    sprintf(game, "GAME plugins                 (currently: %s)", config.gameplugins ? "Disabled" : "Enabled");
    sprintf(pops, "POPS plugins                 (currently: %s)", config.popsplugins ? "Disabled" : "Enabled");
	
	CheckerInitMainScreen("Advanced Configuration Edit Menu");
	hcMenuClear();
    hcMenuAddEntry(patch, 1);
	hcMenuAddEntry(bootbin, 2);
	hcMenuAddEntry(xmb, 3);
	hcMenuAddEntry(game, 4);
	hcMenuAddEntry(pops, 5);
    hcMenuAddEntry("Back", 6);
	
	while(1 == 1)
    {
        btn = hcMenuShowMenu(0, 3);

		if(btn == 1)
        {
			config.umdactivatedplaincheck = !config.umdactivatedplaincheck;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowAdvancedCnfMenu();
        }
        if(btn == 2)
        {
			config.executebootbin = !config.executebootbin;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowAdvancedCnfMenu();
        }	
        if(btn == 3)
        {
			config.xmbplugins = !config.xmbplugins;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowAdvancedCnfMenu();
        }	
        if(btn == 4)
        {
			config.gameplugins = !config.gameplugins;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowAdvancedCnfMenu();
        }
        if(btn == 5)
        {
			config.popsplugins = !config.popsplugins;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowAdvancedCnfMenu();
        }
        if(btn == 6)
        {
		    ShowPage5();
        }		
	}
}

void ShowCnfMenu(void)
{
    int btn;
	
	sctrlSEGetConfig(&config);
	
	sprintf(skip, "Skip Sony logo                 (currently: %s)", config.skiplogo ? "Enabled" : "Disabled");
    sprintf(hide, "Hide corrupt icons             (currently: %s)", config.hidecorrupt ? "Enabled" : "Disabled");
    sprintf(gamefolder, "Game folder homebrew           (currently: %s)", config.gamekernel150 ? "1.50 Kernel" : "6.XX Kernel");
    sprintf(bootprog, "Autoboot program at /PSP/GAME/BOOT/ (currently: %s)", config.startupprog ? "Enabled" : "Disabled");
    sprintf(noumd, "UMD Mode                       (currently: %s)", umdmodes[config.umdmode]);
    sprintf(region, "Fake region                    (currently: %s)", regions[config.fakeregion]);
    sprintf(vshmenu, "Use VshMenu                    (currently: %s)", vshmenuoptions[config.vshmenu]);
    sprintf(usbdev, "XMB Usb Device                 (currently: %s)", usbdevices[config.usbdevice]);
    sprintf(update, "Use network update             (currently: %s)", config.notusedaxupd ? "Disabled" : "Enabled");
    sprintf(hidepic, "Hide PIC1.PNG and PIC0.PNG     (currently: %s)", config.hidepics ? "Enabled" : "Disabled");
    sprintf(versiontxt, "Use version.txt                (currently: %s)", config.useversiontxt ? "Enabled" : "Disabled");
    sprintf(useslimcolor, "Use Slim colors on Classic PSP (currently: %s)", config.slimcolors ? "Enabled" : "Disabled");
    sprintf(hidemac, "Hide MAC address               (currently: %s)", config.hidemac ? "Enabled" : "Disabled");
	
	CheckerInitMainScreen("Configurations Edit Menu");
	hcMenuClear();
    hcMenuAddEntry(skip, 1);
	hcMenuAddEntry(hide, 2);
	hcMenuAddEntry(game, 3);
	hcMenuAddEntry(bootprog, 4);
	hcMenuAddEntry(noumd, 5);
	hcMenuAddEntry(region, 6);
	hcMenuAddEntry(usbdev, 7);
	hcMenuAddEntry(update, 8);
	hcMenuAddEntry(hidepic, 9);
	hcMenuAddEntry(versiontxt, 10);
    hcMenuAddEntry(useslimcolor, 11);
	hcMenuAddEntry(hidemac, 12);
    hcMenuAddEntry("Back", 13);
	
	while(1 == 1)
    {
        btn = hcMenuShowMenu(0, 3);

		if(btn == 1)
        {
			config.skiplogo = !config.skiplogo;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if(btn == 2)
        {
			config.hidecorrupt = !config.hidecorrupt;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }	
        if(btn == 3)
        {
			config.gamekernel150 = !config.gamekernel150;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }	
        if(btn == 4)
        {
			config.startupprog = !config.startupprog;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if(btn == 5)
        {
			if(config.umdmode == MODE_UMD)
                config.umdmode = MODE_MARCH33;
            else if(config.umdmode == MODE_MARCH33)
                config.umdmode = MODE_NP9660;
            else if(config.umdmode == MODE_NP9660)
                config.umdmode = MODE_OE_LEGACY;
            else if(config.umdmode == MODE_OE_LEGACY)
                config.umdmode = MODE_UMD;
				
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if(btn == 6)
        {
			if(config.fakeregion == FAKE_REGION_DISABLED)
                config.fakeregion = FAKE_REGION_JAPAN;
            else if(config.fakeregion == FAKE_REGION_JAPAN)
                config.fakeregion = FAKE_REGION_AMERICA;
            else if(config.fakeregion == FAKE_REGION_AMERICA)
                config.fakeregion = FAKE_REGION_EUROPE;
            else if(config.fakeregion == FAKE_REGION_EUROPE)
                config.fakeregion = FAKE_REGION_KOREA;
            else if(config.fakeregion == FAKE_REGION_KOREA)
                config.fakeregion = FAKE_REGION_UNK;
            else if(config.fakeregion == FAKE_REGION_UNK)
                config.fakeregion = FAKE_REGION_UNK2;
            else if(config.fakeregion == FAKE_REGION_UNK2)
                config.fakeregion = FAKE_REGION_AUSTRALIA;
            else if(config.fakeregion == FAKE_REGION_AUSTRALIA)
                config.fakeregion = FAKE_REGION_HONGKONG;
            else if(config.fakeregion == FAKE_REGION_HONGKONG)
                config.fakeregion = FAKE_REGION_TAIWAN;
            else if(config.fakeregion == FAKE_REGION_TAIWAN)
                config.fakeregion = FAKE_REGION_RUSSIA;
            else if(config.fakeregion == FAKE_REGION_RUSSIA)
                config.fakeregion = FAKE_REGION_CHINA;
            else if(config.fakeregion == FAKE_REGION_CHINA)
                config.fakeregion = FAKE_REGION_DEBUG_TYPE_I;
            else if(config.fakeregion == FAKE_REGION_DEBUG_TYPE_I)
                config.fakeregion = FAKE_REGION_DEBUG_TYPE_II;
            else if(config.fakeregion == FAKE_REGION_DEBUG_TYPE_II)
                config.fakeregion = FAKE_REGION_DISABLED;
				
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if(btn == 7)
        {
			if(config.usbdevice == USBDEVICE_MEMORYSTICK)
                config.usbdevice = USBDEVICE_FLASH0;
            else if(config.usbdevice == USBDEVICE_FLASH0)
                config.usbdevice = USBDEVICE_FLASH1;
            else if(config.usbdevice == USBDEVICE_FLASH1)
                config.usbdevice = USBDEVICE_FLASH2;
            else if(config.usbdevice == USBDEVICE_FLASH2)
                config.usbdevice = USBDEVICE_FLASH3;
            else if(config.usbdevice == USBDEVICE_FLASH3)
                config.usbdevice = USBDEVICE_UMD9660;
            else if(config.usbdevice == USBDEVICE_UMD9660)
                config.usbdevice = USBDEVICE_MEMORYSTICK;
				
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        } 
        if(btn == 8)
        {
			config.notusedaxupd = !config.notusedaxupd;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if(btn == 9)
        {
			config.hidepics = !config.hidepics;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        } 
        if(btn == 10)
        {
			config.useversiontxt = !config.useversiontxt;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        } 
        if(btn == 11)
        {
			config.slimcolors = !config.slimcolors;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        } 
        if(btn == 12)
        {
			config.hidemac = !config.hidemac;
			sceKernelDelayThread(100000);
			sctrlSESetConfig(&config);
			ShowCnfMenu();
        }
        if(btn == 13)
        {
		    ShowPage4();
        }		
	}
}

void ShowSystemMenu(void)
{
    int btn;
	
	CheckerInitMainScreen("System Edit Menu");
	hcMenuClear();
    hcMenuAddEntry("Swap buttons", 1);
	hcMenuAddEntry("Activate WMA", 2);
	hcMenuAddEntry("Activate flash player", 3);
	hcMenuAddEntry("Use Fake name", 4);
	hcMenuAddEntry("Hide MAC Address", 5);
	hcMenuAddEntry("Back", 6);
	
	while(1 == 1)
    {
        btn = hcMenuShowMenu(0, 3);

		if(btn == 1)
        {
		    CheckerInitMainScreen("Assigning...");
			swap_buttons();
			sceKernelDelayThread(2000000);
			ShowSystemMenu();
        }
		if(btn == 2)
        {
		    CheckerInitMainScreen("Activating...");
			active_wma();
			sceKernelDelayThread(2000000);
			ShowSystemMenu();
        }
		if(btn == 3)
        {
		    CheckerInitMainScreen("Activating...");
			active_flash();
			sceKernelDelayThread(2000000);
			ShowSystemMenu();
        }
		if(btn == 4)
        {
		    CheckerInitMainScreen("Faking...");
			fake_name();
			sceKernelDelayThread(2000000);
			ShowSystemMenu();
        }
		if(btn == 5)
        {
		    CheckerInitMainScreen("Hidding...");
			hide_mac();
			sceKernelDelayThread(2000000);
			ShowSystemMenu();
        }
		if(btn == 6)
        {
		    ShowPage3();
        } 
	}
}

void ShowBatteryMenu(void)
{
    int btn;
	
	CheckerInitMainScreen("Battery Edit Menu");
	hcMenuClear();
    hcMenuAddEntry("Make Battery Pandora", 1);
	hcMenuAddEntry("Make Battery AutoBoot", 2);
	hcMenuAddEntry("Make Battery Normal", 3);
	hcMenuAddEntry("Dump Battery Serial to file", 4);
	hcMenuAddEntry("Back", 5);
	
	while(1 == 1)
    {
        btn = hcMenuShowMenu(0, 3);

		if(btn == 1)
        {
		    CheckerInitMainScreen("Creating...");
			
			if(chGetPSPCreatePandora() == 1) Error("Your hardware is not supported for do this");
			
			CheckerPrintf("Creating Pandora Battery...");
			SetBatSer(0xFFFF, 0xFFFF);
	        CheckerPrintf("Done");
			sceKernelDelayThread(2000000);
			ShowBatteryMenu();
        }
		if(btn == 2)
        {
		    CheckerInitMainScreen("Creating...");
			
			if(chGetPSPCreatePandora() == 1) Error("Your hardware is not supported for do this");
			
			CheckerPrintf("Creating AutoBoot Battery...");
			SetBatSer(0x0000, 0x0000);
			CheckerPrintf("Done");
			sceKernelDelayThread(2000000);
			ShowBatteryMenu();
        }
		if(btn == 3)
        {
		    CheckerInitMainScreen("Creating...");
			
			if(chGetPSPCreatePandora() == 1) Error("Your hardware is not supported for do this");
			
			CheckerPrintf("Creating Normal Battery...");
			SetBatSer(0x5241, 0x4E44);
			CheckerPrintf("Done");
			sceKernelDelayThread(2000000);
			ShowBatteryMenu();
        }
		if(btn == 4)
        {
			CheckerInitMainScreen("Dumping...");
			
			if(chGetPSPCreatePandora() == 1) Error("Your hardware is not supported for do this");
			
			CheckerPrintf("Dumping Battery Serial...");
			GetBatSer(BatSerCurrent);
			WriteFile("ms0:/batser.bin", BatSerCurrent, 4);
			CheckerPrintf("Done");
			sceKernelDelayThread(2000000);
			ShowBatteryMenu();
        }
		if(btn == 5)
        {
		    ShowPage2();
        } 
	}
}

void ShowMainMenu(void)
{
    int btn;
	
	CheckerInitMainScreen("Main Menu");
	hcMenuClear();
    hcMenuAddEntry("Start Detection", 1);
	hcMenuAddEntry("Show version.txt", 2);
	
	if(color == 1)
	{
        hcMenuAddEntry("Change text color (currently: Yellow (DEFAULT))", 3);
	}	
    else if(color == 2)
	{
        hcMenuAddEntry("Change text color (currently: Red)", 3);
	}	
    else if(color == 3)
	{
        hcMenuAddEntry("Change text color (currently: Blue)", 3);
	}
    else if(color == 4)
	{
        hcMenuAddEntry("Change text color (currently: Green)", 3);
	}
	else if(color == 5)
	{
        hcMenuAddEntry("Change text color (currently: Black)", 3);
	}
	else if(color == 6)
	{
        hcMenuAddEntry("Change text color (currently: White (NONE))", 3);
	}
	
    hcMenuAddEntry("Exit", 4);
	
	while(1 == 1)
    {
        btn = hcMenuShowMenu(0, 3);

		if(btn == 1)
        {
		    ShowPage1();
        }
		if(btn == 2)
        {
		    ShowVersionTxt();
        } 
		if(btn == 3)
        {
		    if(color == 1)
			{
                color = 2;
				chInitColor("color1.chk", "color2.chk");
				CheckerSetColor(0x000000FF);
				ShowMainMenu();
			}	
            else if(color == 2)
			{
                color = 3;
				chInitColor("color2.chk", "color3.chk");
				CheckerSetColor(0x00FF0000);
				ShowMainMenu();
			}	
            else if(color == 3)
			{
                color = 4;
				chInitColor("color3.chk", "color4.chk");
				CheckerSetColor(0x0000CC00);
				ShowMainMenu();
			}
			else if(color == 4)
			{
                color = 5;
				chInitColor("color4.chk", "color5.chk");
				CheckerSetColor(0x00000000);
				ShowMainMenu();
			}
			else if(color == 5)
			{
                color = 6;
				chInitColor("color5.chk", "color6.chk");
				CheckerSetColor(0x00FFFFFF);
				ShowMainMenu();
			}
			else if(color == 6)
			{
                color = 1;
				chInitColor("color6.chk", "color1.chk");
				CheckerSetColor(0x0000FFFF);
				ShowMainMenu();
			}
        } 
		if(btn == 4)
        {
			CheckerExit();
        }  
	}
}

void ShowCredits()
{
    pspDebugScreenSetTextColor(0x0000CC00);
	CheckerPrintf("Please note theese additional credits...\n\n");
	sceKernelDelayThread(2000000);
	
	pspDebugScreenSetTextColor(0x00FFFFFF);
	CheckerPrintf("Thanks to:\n\n");
	CheckerPrintf("- DAX for his ipl_update.prx\n");
	CheckerPrintf("- Yoti for his libpspident.a\n");
	CheckerPrintf("- Hellcat for his hc menu lib\n");
	CheckerPrintf("- Raing3 for his psppower lib\n\n\n");
	
	CheckerPrintf("Press X to Continue, L to Dont show this again, O to Exit...");
	
	while(1)
	{
	    sceCtrlReadBufferPositive(&pad, 1);

		if (pad.Buttons & PSP_CTRL_CROSS)
		{
		    break;
		}
		if (pad.Buttons & PSP_CTRL_LTRIGGER)
		{
		    chHideMACAddress("dontshow.chk"); //MAC Addr hide can hide credits too...
		}
		if (pad.Buttons & PSP_CTRL_CIRCLE)
		{
		    CheckerExit();
		}
	}  
}

int main(void)
{	
	pspDebugScreenInit();
	
	//Init, check selected colors, We must do it first before printf main screen
	SceUID fd = sceIoOpen("color1.chk", PSP_O_RDONLY, 0777);
	
	if(fd >= 0)
	{
		color = 1;
        CheckerSetColor(0x0000FFFF);
		sceIoClose(fd);
	}
	else
	{
	    sceIoClose(fd);
		fd = sceIoOpen("color2.chk", PSP_O_RDONLY, 0777);
	
	    if(fd >= 0)
	    {
		    color = 2;
            CheckerSetColor(0x000000FF);
		    sceIoClose(fd);
	    }
		else
	    {
	        sceIoClose(fd);
			fd = sceIoOpen("color3.chk", PSP_O_RDONLY, 0777);
	
	        if(fd >= 0)
	        {
		        color = 3;
                CheckerSetColor(0x00FF0000);
		        sceIoClose(fd);
	        }
			else
	        {
	            sceIoClose(fd);
				fd = sceIoOpen("color4.chk", PSP_O_RDONLY, 0777);
	
	            if(fd >= 0)
	            {
		            color = 4;
                    CheckerSetColor(0x0000CC00);
		            sceIoClose(fd);
	            }
				else
	            { 
	                sceIoClose(fd);
				    fd = sceIoOpen("color5.chk", PSP_O_RDONLY, 0777);
	
	                if(fd >= 0)
	                {
		                color = 5;
                        CheckerSetColor(0x00000000);
		                sceIoClose(fd);
	                }
					else
	                { 
	                    sceIoClose(fd);
				        fd = sceIoOpen("color6.chk", PSP_O_RDONLY, 0777);
	
	                    if(fd >= 0)
	                    {
		                    color = 6;
                            CheckerSetColor(0x00FFFFFF);
		                    sceIoClose(fd);
	                    }
	                }
	            }
	        }
	    }
	}
	
	//If colors didnt change select default color (Yellow)
	if(color == 0)
	{
		color = 1;
        CheckerSetColor(0x0000FFFF);
	}
	
	CheckerInitMainScreen("Loading...");
	
	WriteFile("ipl_update.prx", ipl_update, size_ipl_update);
	WriteFile("batman.prx", batman, size_batman);
	WriteFile("kuman.prx", kuman, size_kuman);
	sceKernelDelayThread(1000000);
	
	LoadStartModule("ipl_update.prx");
	LoadStartModule("batman.prx");
	LoadStartModule("kuman.prx");
	sceKernelDelayThread(1000000);
	
	CheckerPrintf("PRXs loaded succefully...\n\n\n");
	sceKernelDelayThread(2000000);
	
	if(VerifyFile("dontshow.chk") == -1) ShowCredits();
	
    ShowMainMenu();
	
	return 0;
}