#include "MGameTypes.h"

void RoleDataToPreview(const FRoleData& From, FPreviewRoleData& To)
{
	To.RoleName = From.RoleName;
	To.Race		= From.Race;
	To.Class	= From.Class;
	To.Rank		= From.Rank;
	To.Gender	= From.Gender;
	To.Birth	= From.Birth;

	// Todo 其它数据
}
