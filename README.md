# Smart City Transport Network 🏙️🚦

A **C++ project** that simulates a **smart city transport network**, allowing users to manage intersections, routes, and predict shortest paths with **AI-based congestion adjustments**.  

This project demonstrates **object-oriented programming**, **graph algorithms**, and **AI-assisted decision making** in a real-world inspired transport network.

---

## Features

### Intersections
- Add, update, or remove intersections in the network
- Each intersection has a unique ID and name

### Routes
- Add, update, or remove routes between intersections
- Automatically maintains bi-directional routes
- Sort routes by distance
- Search for specific routes

### Smart Shortest Path
- Dijkstra's algorithm to find the shortest path between intersections
- AI module predicts traffic congestion based on **time of day**
  - Morning & evening rush hours: 1.5x congestion
  - Moderate traffic hours: 1.2x congestion
  - Low traffic: 1.0x multiplier
- Adjusts route distances dynamically for realistic travel estimates

### User Interface
- Command-line menu for easy interaction:
  1. Add / Update / Remove Intersection
  2. Add / Update / Remove Route
  3. Print Neighbors / Print Full Network
  4. Shortest Path with AI congestion
  5. Sort Routes by Distance
  6. Search for a Route

---

## Technologies Used
- **Language:** C++ (OOP Concepts)  
- **Libraries:** iostream, vector, list, string, queue, algorithm  
- **Tools:** VS Code, Code::Blocks, Qt Creator  
- **Algorithms:** Dijkstra's Shortest Path, Sorting  



