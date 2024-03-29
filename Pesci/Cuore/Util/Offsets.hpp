#pragma once
// Core    
constexpr long OFF_REGION = 0x140000000;                      //[Static]->Region
constexpr long OFF_LEVEL = 0x16f9090;                         //[Miscellaneous]->LevelName
constexpr long OFF_LOCAL_PLAYER = 0x16f9090;                  //[Miscellaneous]->LocalPlayer
constexpr long OFF_ENTITY_LIST = 0x16f9090;                   //[Miscellaneous]->cl_entitylist

constexpr long OFFSET_IN_FORWARD = 0x073f26c8;                       //[Buttons]->in_forward
constexpr long OFFSET_TRAVERSAL_START_TIME = 0x2af0;             //[Buttons]->m_traversalStartTime
constexpr long OFFSET_TRAVERSAL_PROGRESS = 0x2aec;             //[Buttons]->m_traversalProgress
constexpr long OFFSET_WALL_RUN_START_TIME = 0x3584;           //[Buttons]->m_wallRunStartTime  
constexpr long OFFSET_TIME_BASE = 0x2088;                     //m_currentFramePlayer.timeBase   
constexpr long OFF_IN_JUMP = 0x073f2878;                       //[Buttons]->in_jump
constexpr long OFF_IN_DUCK = 0x073f29f8;                       //[Buttons]->in_duck

constexpr long OFF_NAME_LIST = 0xc275bf0;                     //[Miscellaneous]->NameList
constexpr long OFF_NAME_INDEX = 0x38;                         //nameIndex
constexpr long OFF_MODELNAME = 0x0030;                        //m_ModelName
// HUD
constexpr long OFF_VIEWRENDER = 0x73f1578;                    //[Miscellaneous]->ViewRenderer
constexpr long OFF_VIEWMATRIX = 0x11a350;                     //[Miscellaneous]->ViewMatrix

// Player
constexpr long OFF_HEALTH = 0x036c;                           //[RecvTable.DT_Player]->m_iHealth
constexpr long OFF_MAXHEALTH = 0x0460;                        //[RecvTable.DT_Player]->m_iMaxHealth
constexpr long OFF_SHIELD = 0x01a0;                           //[RecvTable.DT_TitanSoul]->m_shieldHealth
constexpr long OFF_MAXSHIELD = 0x01a4;                        //[RecvTable.DT_TitanSoul]->m_shieldHealthMax

constexpr long OFF_INATTACK = 0x073f2818;                     //[Buttons]->in_attack

constexpr long OFF_CAMERAORIGIN = 0x1ed0;                     //[Miscellaneous]->CPlayer!camera_origin
constexpr long OFF_STUDIOHDR = 0xff0;                        //[Miscellaneous]->CBaseAnimating!m_pStudioHdr
constexpr long OFF_BONES = 0x0da0 + 0x48;                     //m_nForceBone

constexpr long OFF_LOCAL_ORIGIN = 0x0004;                     //[DataMap.C_BaseEntity]->m_vecAbsOrigin
constexpr long OFF_ABSVELOCITY = 0x0170;                      //[DataMap.C_BaseEntity]->m_vecAbsVelocity

constexpr long OFF_ZOOMING = 0x1bd1;                          //[RecvTable.DT_Player]->m_bZooming
constexpr long OFF_TEAM_NUMBER = 0x0328;                      //[RecvTable.DT_BaseEntity]->m_iTeamNum
constexpr long OFF_NAME = 0x0471;                             //[RecvTable.DT_BaseEntity]->m_iName
constexpr long OFF_LIFE_STATE = 0x0680;                       //[RecvTable.DT_Player]->m_lifeState
constexpr long OFF_BLEEDOUT_STATE = 0x26e0;                   //[RecvTable.DT_Player]->m_bleedoutState  
constexpr long OFF_LAST_VISIBLE_TIME = 0x198d + 0x3;          //[RecvTable.DT_BaseCombatCharacter]->m_hudInfo_visibilityTestAlwaysPasses + 0x3
constexpr long OFF_LAST_AIMEDAT_TIME = 0x198d + 0x3 + 0x8;    //[RecvTable.DT_BaseCombatCharacter]->m_hudInfo_visibilityTestAlwaysPasses + 0x3 + 0x8
constexpr long OFF_VIEW_ANGLES = 0x2534 - 0x14;               //[DataMap.C_Player]-> m_ammoPoolCapacity - 0x14
constexpr long OFF_PUNCH_ANGLES = 0x2438;                     //[DataMap.C_Player]->m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
constexpr long OFF_YAW = 0x223c - 0x8;                        //m_currentFramePlayer.m_ammoPoolCount - 0x8

// Weapon 
constexpr long OFF_WEAPON_HANDLE = 0x1934;                    //[RecvTable.DT_Player]->m_latestPrimaryWeapons
constexpr long OFF_WEAPON_INDEX = 0x1778;                     //[RecvTable.DT_WeaponX]->m_weaponNameIndex
constexpr long OFF_PROJECTILESCALE = 0x1eac + 0x19e0;         //projectile_gravity_scale + [WeaponSettingsMeta]base
constexpr long OFF_PROJECTILESPEED = 0x1ea4 + 0x19e0;         //projectile_launch_speed + [WeaponSettingsMeta]base
constexpr long OFF_OFFHAND_WEAPON = 0x1944;                   //m_latestNonOffhandWeapons
constexpr long OFF_CURRENTZOOMFOV = 0x15d0 + 0x00b8;          //m_playerData + m_curZoomFOV
constexpr long OFF_TARGETZOOMFOV = 0x15d0 + 0x00bc;           //m_playerData + m_targetZoomFOV

// Glow
constexpr long HIGHLIGHT_TYPE_SIZE = 0x34;  
constexpr long OFF_GLOW_ENABLE = 0x26c;                       //Script_Highlight_GetCurrentContext
constexpr long OFF_GLOW_THROUGH_WALL = 0x26c;                 //Script_Highlight_SetVisibilityType
constexpr long OFF_GLOW_FIX = 0x268;
constexpr long OFF_GLOW_HIGHLIGHT_ID = 0x28C;                 //[DT_HighlightSettings].m_highlightServerActiveStates    
constexpr long OFF_GLOW_HIGHLIGHTS = 0xBB24350;               //HighlightSettings
