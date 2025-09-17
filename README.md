# C++ Console-Raycasting-Engine

This project consists of a simple 3D raycasting engine which is written in C++ that renders a 3D world directly in the windows console using ASCII shading. It is mainly inspired by the early 3D games such as Wolfenstein, yet this was implemented from scratch for learning purposes. 

  Features
- Player movement via W/S and rotation via A/D.
- Renders a 3D wolrd in the Windows console using raycasting. 
- Basic wall shading depending on the distance (eg: closer walls are darker)
- Adjustable map size, render distance and field of view.
- A mini over head map displayed alongside the 3d view.
- Player marked as M on the overhead map.

  Example map
- The map is hardcoded via a grid of characters # for the wall and . for empty space.

  std::wstring map;

  <img width="130" height="112" alt="image" src="https://github.com/user-attachments/assets/cb79d80d-285d-4aa9-9790-041f37f1ac30" />


- This will produce a 10 x 6 map which you can edit the walls # and floor . to create your own level!

How it works? 

Raycasting 
- For each column of the console, a "ray" is casted onto the map
- The ray moves forward towards the wall and once it hits a wall # it stops.
- The distance to the wall is what determines how tall that column of the wall will appear like on the screen.

Shading 
- The closer the walls are the darker the ASCII characters are!
- Further walls have lighter shading.
- The floors are shaded based on the distance for a sense of depth.

Player movement
- Position variables such as fPlayerX, fPlayerY is updated with the sinf and cosf for a smooth motion.
- The collision detection system prevents the player from walking through the walls #.

Requirements 
- Windows only as it uses the Windows.h and console screen buffer.
- A C++ compiler such as MinGW, MSVC, GCC or Clang.

Running the project 
- Clone this repository -> git clone https://github.com/Menu-D/C-Console-Raycasting-Engine.git
- cd raycast-engine
Compile the program (eg: MinGW)
- g++ main.cpp -o raycast.exe -std=c++11
Run
-./raycast.exe

Learning Notes 

This project is a wonderful way to learn how :
- Raycasting works
- Using trigonometry (eg: sine/cosine) to move in a 2D space
- Basics of a game loop and frame timing
- How to manipulate the windows console buffer.

Picture of what the engine looks like 

-<img width="600" height="800" alt="Screenshot 2025-09-17 192014" src="https://github.com/user-attachments/assets/94669875-b5c7-4297-8ddc-35a45565a7bc" />
-<img width="600" height="800" alt="image" src="https://github.com/user-attachments/assets/b409e577-7d99-4c48-9630-803be45f12ee" />





Credits 
- This project was inspired by Javidx9s "ConsoleFps" tutorial series. 
