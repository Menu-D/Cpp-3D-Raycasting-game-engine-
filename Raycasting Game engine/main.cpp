#include <iostream>
#include <chrono> 
#include <Windows.h>
#include <vector>
#include <algorithm>

int nScreenWidth = 120;  // Console Screen size X (columns) 
int nScreenHeight = 40;  // Console Screen size Y (rows) 
int nMapWidth = 10; // World dimensions 
int nMapHeight = 7;

float fPlayerX = 2.0f;  // Player start position 
float fPlayerY = 2.0f;
float fPlayerA = 0.0f; // Player start rotation 
float fFOV = 3.14159 / 4.0; // Field of view 
float fDepth = 50.0; // Maximum rendering distance 

int main()
{
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	std::wstring map;

	map += L"##########";
	map += L"#........#";
	map += L"#...##...#";
	map += L"#...##...#";
	map += L"#...##...#";
	map += L"#...##...#";
	map += L"##########";

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();


	// Main game loop 
	while (1)
	{

		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		// Controls 
		// Handle CCW Rotation 

		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
			fPlayerA -= (0.9f) * fElapsedTime;

		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			fPlayerA += (0.9f) * fElapsedTime;

		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
			{
				fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
			}

		}

		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
			fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;

			if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
			{
				fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
				fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
			}
		}

		for (int x = 0; x < nScreenWidth; x++)
		{
			// For each column, calculate the projected ray angle into world space 
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

			float fDistanceToWall = 0;
			bool bHitWall = false;
			bool bBoundary = false;

			float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space 
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth)
			{

				fDistanceToWall += 0.1f;

				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true; // Set distance to maximum depth 
					fDistanceToWall = fDepth;
				}
				else
				{
					// Ray is inbounds so test to see if the ray cell is a wall block 
					if (map[nTestY * nMapWidth + nTestX] == '#')
					{
						bHitWall = true;

						std::vector<std::pair<float, float>> p; // Distance, dot 

						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								float vy = (float)nTestY + ty - fPlayerY;
								float vx = (float)nTestX + tx - fPlayerX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(std::make_pair(d, dot));
							}
						// Sort the pairs from the closest to the furthest 
						std::sort(p.begin(), p.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right)
							{
								return left.first < right.first;
							});

						float fBound = 0.01;
						if (acos(p.at(0).second) < fBound) bBoundary = true;
						if (acos(p.at(1).second) < fBound) bBoundary = true;
						if (acos(p.at(2).second) < fBound) bBoundary = true;

					}
				}
			}

			// Calculate the distance to ceiling and floor 
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;


			short nWallShade = ' ';
			short nFloorShade = ' ';

			if (fDistanceToWall <= fDepth / 4.0f)                 nWallShade = 0x2588; // Closest 
			else if (fDistanceToWall < fDepth / 3.0f)             nWallShade = 0x2593;
			else if (fDistanceToWall < fDepth / 2.0f)             nWallShade = 0x2592;
			else if (fDistanceToWall < fDepth)                    nWallShade = 0x2591;
			else                                                  nWallShade = ' '; // Furthest 

			if (bBoundary)  nWallShade = ' '; // Black it out

			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y < nCeiling)
					screen[y * nScreenWidth + x] = ' ';
				else if (y > nCeiling && y <= nFloor)
					screen[y * nScreenWidth + x] = nWallShade;
				else
				{
					// Shading the floor based on the distance to the wall 
					float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					if (b < 0.25)             nFloorShade = '.';
					else if (b < 0.5)         nFloorShade = '.';
					else if (b < 0.75)        nFloorShade = '.';
					else if (b < 0.9)         nFloorShade = '_';
					else                      nFloorShade = ' ';
					screen[y * nScreenWidth + x] = nFloorShade;
				}

			}

		}
		// Display Stats 
		swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS =%3.2f", fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime); // lambda function 


		//Display Map 
		for (int nx = 0; nx < nMapWidth; nx++)
			for (int ny = 0; ny < nMapHeight; ny++)
			{
				screen[(ny + 1) * nScreenWidth + nx] = map[ny * nMapWidth + nx];

			}

		screen[((int)fPlayerY + 1) * nScreenWidth + (int)fPlayerX] = 'M';
		screen[nScreenWidth * nScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}

