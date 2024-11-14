#include "stdafx.h"
#include <vector>
#include <map>
#include <string>
#include "Tech.h"
#include "UI.h"

std::map<std::string, FieldType> g_pFields;

TechFile *TechFile::gTechFile = 0;

bool TechFile::LoadTech(TCHAR *filename, HWND cb, HWND lb, HWND *arr, HWND lbl)
{
	FILE *f = _tfopen(filename, "rb");
	if (!f)
		return false;

	if (gTechFile)
		delete gTechFile;

	gTechFile = new TechFile();

	gTechFile->currentItem = 0;
	gTechFile->currentField = "";

	fread(&gTechFile->m_pFileData.len, 4, 1, f);
	gTechFile->m_pFileData.filename = new char[gTechFile->m_pFileData.len];

	fread(gTechFile->m_pFileData.filename, 1, gTechFile->m_pFileData.len, f);
	fread(&gTechFile->m_pFileData.dup0, 4, 1, f);
	fread(&gTechFile->m_pFileData.size, 4, 1, f);
	fread(&gTechFile->m_pFileData.count, 4, 1, f);
	for (int i = 0; i < 7; i++)
	{
		fread(&gTechFile->m_pFileData.dup[i], 4, 1, f);
	}
	for (int i = 0; i < 80; i++)
	{
		fread(&gTechFile->m_pFileData.techData[i], 1, 0x970, f);
	}

	gTechFile->m_pFileData.dup2 = new DWORD[gTechFile->m_pFileData.count];
	for (int i = 0; i < gTechFile->m_pFileData.count; i++)
	{
		fread(&gTechFile->m_pFileData.dup2[i], 4, 1, f);
	}

	fread(&gTechFile->m_pFileData.addSize, 4, 1, f);
	fread(&gTechFile->m_pFileData.addCount, 4, 1, f);

	for (int i = 0; i < 7; i++)
	{
		fread(&gTechFile->m_pFileData.dup3[i], 4, 1, f);
	}

	fread(&gTechFile->m_pFileData.techDataAdd, 0x36C, 0x23, f);

	fclose(f);

	for (int i = 0; i < gTechFile->m_pFileData.count; i++)
	{
		UI_ComboBoxAddLine(cb, gTechFile->m_pFileData.techData[i].prefix);
	}

	gTechFile->cb = cb;
	gTechFile->lb = lb;
	gTechFile->arr = arr;
	gTechFile->lbl = lbl;

	gTechFile->m_sFileName = new TCHAR[strlen(filename) + 1];
	strcpy(gTechFile->m_sFileName, filename);
	return true;
}

void TechFile::LoadItem(int id)
{
	if (gTechFile)
	{
		if (id >= 0)
		{
			UI_ResetListBox(gTechFile->lb);
			for (auto it = g_pFields.begin(); it != g_pFields.end(); it++)
			{
				UI_ListBoxAddLine(gTechFile->lb, it->first.c_str());
			}
		}
		gTechFile->currentItem = id;
	}
}

void TechFile::LoadField(std::string id)
{
	std::string tmpType;
	std::string data;

	for (int i = 0; i < 14; i++)
	{
		ShowWindow(gTechFile->arr[i], 0);
	}
	gTechFile->currentField = id;

	if (id == "")
	{
		SetWindowText(gTechFile->lbl, "");
		return;
	}

	FieldType type = g_pFields[id];
	switch (type.type)
	{
	case 1:
		tmpType = "int";
		if (type.size > 1)
		{
			tmpType += "[" + std::to_string(type.size) + "]";
		}
		SetWindowText(gTechFile->lbl, tmpType.c_str());
		for (int i = 0; i < type.size; i++)
		{
			ShowWindow(gTechFile->arr[i], 1);
			data = std::to_string(((int *)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] +type.offset))[i]);
			SetWindowText(gTechFile->arr[i], data.c_str());
		}
		
		break;

	case 2:
		tmpType = "float";
		if (type.size > 1)
		{
			tmpType += "[" + std::to_string(type.size) + "]";
		}
		SetWindowText(gTechFile->lbl, tmpType.c_str());
		for (int i = 0; i < type.size; i++)
		{
			ShowWindow(gTechFile->arr[i], 1);
			data = std::to_string(((float *)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset))[i]);
			SetWindowText(gTechFile->arr[i], data.c_str());
		}
		break;
		
	case 3:
		SetWindowText(gTechFile->lbl, "Vector3D");
		for (int i = 0; i < 3; i++)
		{
			ShowWindow(gTechFile->arr[i], 1);
			data = std::to_string(((float *)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset))[i]);
			SetWindowText(gTechFile->arr[i], data.c_str());
		}
		break;

	case 4:
		SetWindowText(gTechFile->lbl, "string");
		ShowWindow(gTechFile->arr[0], 1);
		SetWindowText(gTechFile->arr[0], (char*)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset));
		break;
	}
}

void TechFile::SaveFields()
{
	char buffer[1024];
	memset(buffer, 0, 1024);
	if (gTechFile->currentItem >= 0 && !gTechFile->currentField.empty())
	{
		FieldType type = g_pFields[gTechFile->currentField];
		switch (type.type)
		{
		case 1:
			for (int i = 0; i < type.size; i++)
			{
				GetWindowText(gTechFile->arr[i], buffer, 1024);
				((int *)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset))[i] = atoi(buffer);
				SetWindowText(gTechFile->arr[i], std::to_string(((int *)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset))[i]).c_str());
			}

			break;

		case 2:
			for (int i = 0; i < type.size; i++)
			{
				GetWindowText(gTechFile->arr[i], buffer, 1024);
				((float *)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset))[i] = atof(buffer);
				SetWindowText(gTechFile->arr[i], std::to_string(((float *)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset))[i]).c_str());
			}
			break;

		case 3:
			for (int i = 0; i < 3; i++)
			{
				GetWindowText(gTechFile->arr[i], buffer, 1024);
				((float *)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset))[i] = atof(buffer);
				SetWindowText(gTechFile->arr[i], std::to_string(((float *)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset))[i]).c_str());
			}
			break;

		case 4:
			GetWindowText(gTechFile->arr[0], buffer, 1024);
			strncpy((char*)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset), buffer, type.size);
			((char*)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset))[strlen(buffer)] = 0;
			SetWindowText(gTechFile->arr[0], (char*)((DWORD)&gTechFile->m_pFileData.techData[gTechFile->currentItem] + type.offset));
			break;
		}
	}
}

bool TechFile::Save(char *filename)
{
	if (!filename)
		filename = gTechFile->m_sFileName;

	FILE *f = fopen(filename, "wb");
	if (!f)
		return false;

	fwrite(&gTechFile->m_pFileData.len, 4, 1, f);

	fwrite(gTechFile->m_pFileData.filename, 1, gTechFile->m_pFileData.len, f);
	fwrite(&gTechFile->m_pFileData.dup0, 4, 1, f);
	fwrite(&gTechFile->m_pFileData.size, 4, 1, f);
	fwrite(&gTechFile->m_pFileData.count, 4, 1, f);

	for (int i = 0; i < 7; i++)
	{
		fwrite(&gTechFile->m_pFileData.dup[i], 4, 1, f);
	}
	for (int i = 0; i < 80; i++)
	{
		fwrite(&gTechFile->m_pFileData.techData[i], 1, 0x970, f);
	}

	for (int i = 0; i < gTechFile->m_pFileData.count; i++)
	{
		fwrite(&gTechFile->m_pFileData.dup2[i], 4, 1, f);
	}

	fwrite(&gTechFile->m_pFileData.addSize, 4, 1, f);
	fwrite(&gTechFile->m_pFileData.addCount, 4, 1, f);

	for (int i = 0; i < 7; i++)
	{
		fwrite(&gTechFile->m_pFileData.dup3[i], 4, 1, f);
	}

	fwrite(&gTechFile->m_pFileData.techDataAdd, 0x36C, 0x23, f);

	fclose(f);
	return true;
}

void TechFile::Init()
{
	FieldType type = { 1, 1, 0 };
	type.offset = 0x90;
	g_pFields.emplace("gearsCount", type);
	type.offset = 0x7C4;
	g_pFields.emplace("horse_power", type);
	type.offset = 0x7C8;
	g_pFields.emplace("price", type);
	type.offset = 0x7CC;
	g_pFields.emplace("glowwing", type);
	type.offset = 0x948;
	g_pFields.emplace("trailer_type", type);
	type.offset = 0x730;
	g_pFields.emplace("tvCameraPositionCount", type);
	type.offset = 0x574;
	g_pFields.emplace("Corner_Mark_WCount", type);
	type.offset = 0x368;
	g_pFields.emplace("Corner_Mark_Count", type);
	type.offset = 0x36C;
	g_pFields.emplace("AddColPntsCount", type);
	type.size = 8;
	type.offset = 0x12C;
	g_pFields.emplace("driving_wheels", type);
	type.offset = 0x14C;
	g_pFields.emplace("steering_wheels", type);
	type.offset = 0x16C;
	g_pFields.emplace("touching_road_wheels", type);



	type.type = 2;
	type.size = 1;
	type.offset = 0x60;
	g_pFields.emplace("mass", type);
	type.offset = 0x64;
	g_pFields.emplace("rwheel", type);
	type.offset = 0x68;
	g_pFields.emplace("rwheel1", type);
	type.offset = 0x6C;
	g_pFields.emplace("tank", type);
	type.offset = 0x70;
	g_pFields.emplace("fuelTimeEndurance", type);
	type.offset = 0xA0;
	g_pFields.emplace("fuelConsumption", type);
	type.offset = 0xA8;
	g_pFields.emplace("kilometrage", type);
	type.offset = 0xAC;
	g_pFields.emplace("air_press_coeff", type);
	type.offset = 0xB0;
	g_pFields.emplace("air_resist_coeff", type);
	type.offset = 0xB4;
	g_pFields.emplace("air_side_resist_coeff", type);
	type.offset = 0xB8;
	g_pFields.emplace("front_wheel_thickness", type);
	type.offset = 0xBC;
	g_pFields.emplace("rear_wheel_thickness", type);
	type.offset = 0xC0;
	g_pFields.emplace("maxrotmomentum", type);
	type.offset = 0xC8;
	g_pFields.emplace("breakpower", type);
	type.offset = 0xCC;
	g_pFields.emplace("rear_axle_coeff", type);
	type.offset = 0xD0;
	g_pFields.emplace("reverse_trans_coeff", type);
	type.offset = 0x10C;
	g_pFields.emplace("tach_with_max_power", type);
	type.offset = 0x110;
	g_pFields.emplace("cargo_and_body_pick_load", type);
	type.offset = 0x114;
	g_pFields.emplace("body_mass", type);
	type.offset = 0x118;
	g_pFields.emplace("body_height", type);
	type.offset = 0x11C;
	g_pFields.emplace("body_square", type);
	type.offset = 0x198;
	g_pFields.emplace("tangViewer", type);
	type.offset = 0x34C;
	g_pFields.emplace("CollisionRadius", type);
	type.offset = 0x360;
	g_pFields.emplace("rubber_collision_coeff", type);
	type.offset = 0x364;
	g_pFields.emplace("slide_collision_coeff", type);
	type.offset = 0x800;
	g_pFields.emplace("shiftSaddle", type);
	type.offset = 0x804;
	g_pFields.emplace("CouplerLength", type);
	type.offset = 0x818;
	g_pFields.emplace("FuelScale", type);

	type.size = 14;
	type.offset = 0xD4;
	g_pFields.emplace("gear_trans_coeff", type);
	type.size = 8;
	type.offset = 0x2EC;
	g_pFields.emplace("suspension", type);
	type.offset = 0x30C;
	g_pFields.emplace("ks", type);
	type.offset = 0x32C;
	g_pFields.emplace("cs", type);
	type.size = 4;
	type.offset = 0x834;
	g_pFields.emplace("CabFrontWindow", type);
	type.offset = 0x5A8;
	g_pFields.emplace("CollisionPlane", type);
	type.size = 7;
	type.offset = 0x844;
	g_pFields.emplace("Drivers_neck_Angles", type);

	type.size = 1;
	type.type = 3;
	type.offset = 0x20;
	g_pFields.emplace("b3d_mass_center", type);
	type.offset = 0x120;
	g_pFields.emplace("center_of_cargo_bottom", type);
	type.offset = 0x18C;
	g_pFields.emplace("OV", type);
	type.offset = 0x19C;
	g_pFields.emplace("I", type);
	type.offset = 0x1A8;
	g_pFields.emplace("rs0", type);
	type.offset = 0x1B4;
	g_pFields.emplace("rs1", type);
	type.offset = 0x1C0;
	g_pFields.emplace("rs2", type);
	type.offset = 0x1C;
	g_pFields.emplace("rs3", type);
	type.offset = 0x350;
	g_pFields.emplace("CollisionCenter", type);
	type.offset = 0x740;
	g_pFields.emplace("SmokePosition", type);
	type.offset = 0x74C;
	g_pFields.emplace("SmokeDirection", type);
	type.offset = 0x758;
	g_pFields.emplace("SmokePosition2", type);
	type.offset = 0x764;
	g_pFields.emplace("SmokeDirection2", type);
	type.offset = 0x770;
	g_pFields.emplace("flame_place", type);
	type.offset = 0x77C;
	g_pFields.emplace("Fara_FL", type);
	type.offset = 0x788;
	g_pFields.emplace("Fara_FR", type);
	type.offset = 0x794;
	g_pFields.emplace("Fara_BLs", type);
	type.offset = 0x7A0;
	g_pFields.emplace("Fara_BRs", type);
	type.offset = 0x7AC;
	g_pFields.emplace("Fara_BL", type);
	type.offset = 0x7B8;
	g_pFields.emplace("Fara_BR", type);
	type.offset = 0x7D0;
	g_pFields.emplace("lowSaddle", type);
	type.offset = 0x7DC;
	g_pFields.emplace("topSaddle", type);
	type.offset = 0x7E8;
	g_pFields.emplace("TowHook", type);
	type.offset = 0x7F4;
	g_pFields.emplace("Coupler", type);
	type.offset = 0x81C;
	g_pFields.emplace("CabRWindow", type);
	type.offset = 0x828;
	g_pFields.emplace("CabLWindow", type);
	type.offset = 0x94C;
	g_pFields.emplace("Fara_Dir", type);
	type.offset = 0x958;
	g_pFields.emplace("Fara_Dir2", type);
	type.offset = 0x964;
	g_pFields.emplace("Fara_Width", type);
	type.offset = 0x6F4;
	g_pFields.emplace("tvCameraPosition1", type);
	type.offset = 0x6F4 + 0xC;
	g_pFields.emplace("tvCameraPosition2", type);
	type.offset = 0x6F4 + 0xC * 2;
	g_pFields.emplace("tvCameraPosition3", type);
	type.offset = 0x6F4 + 0xC * 3;
	g_pFields.emplace("tvCameraPosition4", type);
	type.offset = 0x6F4 + 0xC * 4;
	g_pFields.emplace("tvCameraPosition5", type);
	type.offset = 0x578;
	g_pFields.emplace("Corner_Mark_W1", type);
	type.offset = 0x578 + 0xC;
	g_pFields.emplace("Corner_Mark_W2", type);
	type.offset = 0x578 + 0xC * 2;
	g_pFields.emplace("Corner_Mark_W3", type);
	type.offset = 0x578 + 0xC * 3;
	g_pFields.emplace("Corner_Mark_W4", type);
	for (int i = 0; i < 20; i++)
	{
		type.offset = 0x370 + 0xC * i;
		g_pFields.emplace("Corner_Mark"+std::to_string(i+1), type);
	}
	for (int i = 0; i < 20; i++)
	{
		type.offset = 0x460 + 0xC * i;
		g_pFields.emplace("AddColPnts" + std::to_string(i+1), type);
	}

	type.type = 4;
	type.size = 60;
	type.offset = 0x868;
	g_pFields.emplace("cockpit_space", type);
	type.size = 30;
	type.offset = 0x8A4;
	g_pFields.emplace("fileName", type);
	type.size = 20;
	type.offset = 0x8FE;
	g_pFields.emplace("prefix", type);
	type.size = 20;
	type.offset = 0x912;
	g_pFields.emplace("prefixCab", type);
	type.size = 20;
	type.offset = 0x926;
	g_pFields.emplace("car_node", type);
	 
}

TechFile::TechFile()
{
	m_pFileData.filename = 0;
	m_pFileData.dup2 = 0;
}

TechFile::~TechFile()
{
	if (m_pFileData.filename)
		delete[] m_pFileData.filename;

	if (m_pFileData.dup2)
		delete[] m_pFileData.dup2;

	if (m_sFileName)
		delete[] m_sFileName;
}