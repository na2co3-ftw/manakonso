
#ifndef VERSION_H
#define VERSION_H

#define TEXTSERVICE_NAME	L"manakonso"
#define TEXTSERVICE_NAME_ALT	L"京極入致"
#ifndef _DEBUG
#define TEXTSERVICE_DESC	TEXTSERVICE_NAME
#define TEXTSERVICE_DESC_ALT	TEXTSERVICE_NAME_ALT
#else
#define TEXTSERVICE_DESC	TEXTSERVICE_NAME L"_DEBUG"
#define TEXTSERVICE_DESC_ALT	TEXTSERVICE_NAME_ALT L"_DEBUG"
#endif
#define TEXTSERVICE_DIR		L"IMMANAKONSO"
#define TEXTSERVICE_VER		L"0.1.0"

//for resource
#define RC_AUTHOR			"na2co3"
#define RC_PRODUCT			"京極入致"
#define RC_VERSION			"0.1.0"
#define RC_VERSION_D		0,1,0,0

#endif
