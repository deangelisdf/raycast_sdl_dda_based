#include "RayCastTileMapManager.h"
#include <cmath>


bool RayCastTileMapManager::raycast(const SDL_Point& pStart, const SDL_FPoint& pfRayDir, float fMaxDistance, SDL_FPoint& pfFounded)
{
	bool bTileFound = false;
	float fDistance = 0.0f;
	SDL_Point vMapCheck = pStart;
	SDL_Point vStep = { 0, 0 };
	SDL_FPoint vRayLength1D = { 0.0f, 0.0f };
	SDL_FPoint vRayUnitStepSize = { (abs(1.0f / pfRayDir.x)), (abs(1.0f / pfRayDir.y)) };

	// Establish Starting Conditions
	if (pfRayDir.x < 0)
	{
		vStep.x = -1;
		vRayLength1D.x = 0;
	}
	else
	{
		vStep.x = 1;
		vRayLength1D.x = vRayUnitStepSize.x;
	}

	if (pfRayDir.y < 0)
	{
		vStep.y = -1;
		vRayLength1D.y = 0;
	}
	else
	{
		vStep.y = 1;
		vRayLength1D.y = vRayUnitStepSize.y;
	}

	// Perform "Walk" until collision or range check
	while (!bTileFound && fDistance < fMaxDistance)
	{
		// Walk along shortest path
		if (vRayLength1D.x < vRayLength1D.y)
		{
			vMapCheck.x += vStep.x;
			fDistance = vRayLength1D.x;
			vRayLength1D.x += vRayUnitStepSize.x;
		}
		else
		{
			vMapCheck.y += vStep.y;
			fDistance = vRayLength1D.y;
			vRayLength1D.y += vRayUnitStepSize.y;
		}
		if (vMapCheck.x >= MAX_DIM || vMapCheck.y >= MAX_DIM) //in boundary
		{
			break;
		}

		// Test tile at new test point
		if (vMapCheck.x >= 0 && vMapCheck.x < this->map_width && vMapCheck.y >= 0 && vMapCheck.y < this->map_height)
		{
			if (this->matrix[vMapCheck.x][vMapCheck.y])
			{
				bTileFound = true;
			}
		}
	}

	// Calculate intersection location
	if (bTileFound)
	{
		pfFounded.x = (float)(pStart.x) + (float)(pfRayDir.x * fDistance);
		pfFounded.y = (float)(pStart.y) + (float)(pfRayDir.y * fDistance);
	}

	return bTileFound;
}


bool RayCastTileMapManager::raycast(const SDL_Point& pStart, float fAngle, float fMaxDistance, SDL_FPoint& pfFounded)
{
	bool bTileFound = false;
	float fDistance = 0.0f;
	SDL_Point vMapCheck = pStart;
	SDL_Point vStep = { 0, 0 };
	SDL_FPoint vRayLength1D = { 0.0f, 0.0f };
	SDL_FPoint vRayDir = { cos(fAngle), sin(fAngle) };
	SDL_FPoint vRayUnitStepSize = { (abs(1.0f / vRayDir.x)), (abs(1.0f / vRayDir.y)) };
	
	// Establish Starting Conditions
	if (vRayDir.x < 0)
	{
		vStep.x = -1;
		vRayLength1D.x = 0;
	}
	else
	{
		vStep.x = 1;
		vRayLength1D.x = vRayUnitStepSize.x;
	}

	if (vRayDir.y < 0)
	{
		vStep.y = -1;
		vRayLength1D.y = 0;
	}
	else
	{
		vStep.y = 1;
		vRayLength1D.y = vRayUnitStepSize.y;
	}

	// Perform "Walk" until collision or range check
	while (!bTileFound && fDistance < fMaxDistance)
	{
		// Walk along shortest path
		if (vRayLength1D.x < vRayLength1D.y)
		{
			vMapCheck.x += vStep.x;
			fDistance = vRayLength1D.x;
			vRayLength1D.x += vRayUnitStepSize.x;
		}
		else
		{
			vMapCheck.y += vStep.y;
			fDistance = vRayLength1D.y;
			vRayLength1D.y += vRayUnitStepSize.y;
		}
		if (vMapCheck.x >= MAX_DIM || vMapCheck.y >= MAX_DIM) //in boundary
		{
			break;
		}

		// Test tile at new test point
		if (vMapCheck.x >= 0 && vMapCheck.x < this->map_width && vMapCheck.y >= 0 && vMapCheck.y < this->map_height)
		{
			if (this->matrix[vMapCheck.x][vMapCheck.y])
			{
				bTileFound = true;
			}
		}
	}

	// Calculate intersection location
	if (bTileFound)
	{
		pfFounded.x = (float)(pStart.x) + (float)(vRayDir.x * fDistance);
		pfFounded.y = (float)(pStart.y) + (float)(vRayDir.y * fDistance);
	}

    return bTileFound;
}
