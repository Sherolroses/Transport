#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <limits>
#include <algorithm>
using namespace std;

//=========================== Route Class =========================//
class Route { // Represents a route between intersections
public:
    int distance;              // Distance to the destination intersection    
    int IntersectionDestinationID; // ID of the destination intersection

    Route() { distance = 0; IntersectionDestinationID = 0; }
    Route(int IntersectionDestID, int Dist) { 
        IntersectionDestinationID = IntersectionDestID; 
        distance = Dist; 
    }

    void setIntersectionDestinationID(int IntersectionDestID) { IntersectionDestinationID = IntersectionDestID; }
    int getIntersectionDestinationID() { return IntersectionDestinationID; }

    void setDistance(int dist) { distance = dist; }
    int getDistance() { return distance; }
};

//=========================== Intersection Class =========================//
class Intersection { // Represents an intersection/city in the transport network
public:
    string IntersectionName;
    int IntersectionID;
    list<Route> neighbors;

    Intersection() { IntersectionName = " "; IntersectionID = 0; }
    Intersection(int IID, string Iname) { IntersectionID = IID; IntersectionName = Iname; }

    void setIntersectionName(string Iname) { IntersectionName = Iname; }
    string getIntersectionName() { return IntersectionName; }

    void setIntersectionID(int IID) { IntersectionID = IID; }
    int getIntersectionID() { return IntersectionID; }

    list<Route> getNeighbors() { return neighbors; }

    void printNeighbors() {
        for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
            cout << "[" << it->getIntersectionDestinationID() << "," << it->getDistance() << "] ";
        }
        cout << endl;
    }
};

//=========================== AI Module =========================//
// Predict congestion factor based on time of day
// Returns a multiplier for route distances (1 = normal, >1 = congested)
double PredictCongestion(int hour) {
    if (hour >= 7 && hour <= 9) return 1.5;  // Morning rush
    if (hour >= 16 && hour <= 18) return 1.5; // Evening rush
    if (hour > 9 && hour < 16) return 1.2;   // Moderate traffic
    return 1.0;                              // Low traffic
}

//=========================== TransportNetwork Class =========================//
class TransportNetwork {
public:
    vector<Intersection> Intersections;

    void AddIntersection(Intersection newIntersection) {
        if (CheckIfIntersectionExists(newIntersection.getIntersectionID())) {
            cout << "Intersection with ID " << newIntersection.getIntersectionID() << " already exists.\n";
        } else {
            Intersections.push_back(newIntersection);
            cout << "Intersection with ID " << newIntersection.getIntersectionID() << " added.\n";
        }
    }

    void UpdateIntersection(int IntersectionID, string newName) {
        for (auto &c : Intersections) {
            if (c.getIntersectionID() == IntersectionID) {
                c.setIntersectionName(newName);
                cout << "Intersection updated: " << IntersectionID << " -> " << newName << endl;
                return;
            }
        }
        cout << "Intersection ID not found.\n";
    }

    void RemoveIntersection(int IntersectionID) {
        bool found = false;
        for (int i = 0; i < Intersections.size(); ++i) {
            if (Intersections[i].getIntersectionID() == IntersectionID) {
                Intersections.erase(Intersections.begin() + i);
                found = true;
                break;
            }
        }
        if (!found) {
            cout << "Intersection ID " << IntersectionID << " does not exist.\n";
            return;
        }
        for (auto &c : Intersections) {
            for (auto it = c.neighbors.begin(); it != c.neighbors.end();) {
                if (it->getIntersectionDestinationID() == IntersectionID) 
                    it = c.neighbors.erase(it);
                else ++it;
            }
        }
        cout << "Intersection " << IntersectionID << " and its routes removed.\n";
    }

    void AddRoute(int fromID, int toID, int distance) {
        Intersection *fromIntersection = nullptr, *toIntersection = nullptr;
        for (auto &c : Intersections) {
            if (c.getIntersectionID() == fromID) fromIntersection = &c;
            if (c.getIntersectionID() == toID) toIntersection = &c;
        }
        if (!fromIntersection || !toIntersection) {
            cout << "One or both intersections not found.\n";
            return;
        }
        fromIntersection->neighbors.push_back(Route(toID, distance));
        toIntersection->neighbors.push_back(Route(fromID, distance));
        cout << "Route added: " << fromID << " <-> " << toID << " | Distance: " << distance << endl;
    }

    void UpdateRoute(int fromID, int toID, int newDistance) {
        for (auto &c : Intersections) {
            if (c.getIntersectionID() == fromID) {
                for (auto &r : c.neighbors)
                    if (r.getIntersectionDestinationID() == toID) r.setDistance(newDistance);
            }
            if (c.getIntersectionID() == toID) {
                for (auto &r : c.neighbors)
                    if (r.getIntersectionDestinationID() == fromID) r.setDistance(newDistance);
            }
        }
        cout << "Route updated: " << fromID << " <-> " << toID << " | New Distance: " << newDistance << endl;
    }

    void RemoveRoute(int fromID, int toID) {
        for (auto &c : Intersections) {
            if (c.getIntersectionID() == fromID) {
                for (auto it = c.neighbors.begin(); it != c.neighbors.end();) {
                    if (it->getIntersectionDestinationID() == toID) 
                        it = c.neighbors.erase(it);
                    else ++it;
                }
            }
            if (c.getIntersectionID() == toID) {
                for (auto it = c.neighbors.begin(); it != c.neighbors.end();) {
                    if (it->getIntersectionDestinationID() == fromID) 
                        it = c.neighbors.erase(it);
                    else ++it;
                }
            }
        }
        cout << "Route removed: " << fromID << " <-> " << toID << endl;
    }

    void printNeighbors(int IntersectionID) {
        for (auto &c : Intersections) {
            if (c.getIntersectionID() == IntersectionID) {
                cout << "Neighbors of " << c.getIntersectionName() << ":\n";
                c.printNeighbors();
                return;
            }
        }
        cout << "Intersection not found.\n";
    }

    void printTransportNetwork() {
        cout << "\n--- Transport Network ---\n";
        for (auto &c : Intersections) {
            cout << c.getIntersectionName() << "(" << c.getIntersectionID() << ") --> ";
            c.printNeighbors();
        }
    }

    //================== Dijkstra with AI Congestion ==================//
    void ShortestPath(int startID, int endID, int currentHour) {
        double congestionMultiplier = PredictCongestion(currentHour);
        cout << "[XAI] Congestion multiplier for hour " << currentHour << ": " 
             << congestionMultiplier << endl;

        int n = Intersections.size();
        vector<double> dist(n, numeric_limits<double>::max());
        vector<int> prev(n, -1);
        vector<bool> visited(n, false);

        auto idToIndex = [&](int id) {
            for (int i = 0; i < Intersections.size(); i++)
                if (Intersections[i].getIntersectionID() == id) return i;
            return -1;
        };

        int startIdx = idToIndex(startID);
        int endIdx = idToIndex(endID);
        if (startIdx == -1 || endIdx == -1) { 
            cout << "Invalid intersection IDs.\n"; return; 
        }

        dist[startIdx] = 0;

        for (int i = 0; i < n; i++) {
            int u = -1; double minDist = numeric_limits<double>::max();
            for (int j = 0; j < n; j++)
                if (!visited[j] && dist[j] < minDist) { u = j; minDist = dist[j]; }

            if (u == -1) break;
            visited[u] = true;

            for (auto &r : Intersections[u].neighbors) {
                int v = idToIndex(r.getIntersectionDestinationID());
                if (v != -1) {
                    double adjustedDist = r.getDistance() * congestionMultiplier; // AI adjustment
                    if (dist[u] + adjustedDist < dist[v]) {
                        dist[v] = dist[u] + adjustedDist;
                        prev[v] = u;

                        cout << "[XAI] Updating distance to Intersection " 
                             << Intersections[v].getIntersectionID()
                             << " via Intersection " << Intersections[u].getIntersectionID()
                             << " | Original distance: " << r.getDistance() 
                             << " | Adjusted for congestion: " << adjustedDist 
                             << " | New total: " << dist[v] << endl;
                    }
                }
            }
        }

        if (dist[endIdx] == numeric_limits<double>::max()) {
            cout << "No path found.\n"; return;
        }

        vector<int> path;
        for (int at = endIdx; at != -1; at = prev[at]) path.push_back(Intersections[at].getIntersectionID());
        reverse(path.begin(), path.end());

        cout << "Shortest path considering congestion: ";
        for (int i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i != path.size() - 1) cout << " -> ";
        }
        cout << " | Total adjusted distance: " << dist[endIdx] << endl;
    }

    void SortRoutesByDistance(int IntersectionID) {
        for (auto &c : Intersections) {
            if (c.getIntersectionID() == IntersectionID) {
                vector<Route> routeVec(c.neighbors.begin(), c.neighbors.end());
                sort(routeVec.begin(), routeVec.end(), [](Route a, Route b) { return a.getDistance() < b.getDistance(); });
                cout << "Sorted routes for " << c.getIntersectionName() << ":\n";
                for (auto &r : routeVec) 
                    cout << "To Intersection " << r.getIntersectionDestinationID() 
                         << " | Distance: " << r.getDistance() << endl;
                return;
            }
        }
        cout << "Intersection not found.\n";
    }

    void SearchRoute(int fromID, int toID) {
        for (auto &c : Intersections) {
            if (c.getIntersectionID() == fromID) {
                auto it = find_if(c.neighbors.begin(), c.neighbors.end(), [toID](Route r) { return r.getIntersectionDestinationID() == toID; });
                if (it != c.neighbors.end())
                    cout << "Route exists: " << fromID << " -> " << toID << " | Distance: " << it->getDistance() << endl;
                else
                    cout << "Route not found between " << fromID << " and " << toID << endl;
                return;
            }
        }
        cout << "From intersection not found.\n";
    }

private:
    bool CheckIfIntersectionExists(int IntersectionID) {
        for (auto &c : Intersections) 
            if (c.getIntersectionID() == IntersectionID) return true;
        return false;
    }
};

//=========================== Main Function =========================//
int main() {
    TransportNetwork Network;

    // Preloaded intersections
    Network.AddIntersection(Intersection(0, "CapeTown_CBD"));
    Network.AddIntersection(Intersection(1, "Observatory"));
    Network.AddIntersection(Intersection(2, "Rondebosch"));
    Network.AddIntersection(Intersection(3, "Claremont"));

    // Preloaded routes
    // Distances are in km
    
    Network.AddRoute(0, 1, 6);
    Network.AddRoute(1, 2, 3);
    Network.AddRoute(2, 3, 2);
    Network.AddRoute(0, 3, 10);

    int operation, id1, id2, dist;
    string Iname;
    int hour;

    do {
        cout << "\n--- Smart City Route Menu ---\n";
        cout << "1. Add Intersection\n2. Update Intersection\n3. Remove Intersection\n";
        cout << "4. Add Route\n5. Update Route\n6. Remove Route\n";
        cout << "7. Print Neighbors\n8. Print Transport Network\n";
        cout << "9. Shortest Path (Dijkstra with AI)\n10. Sort Routes by Distance\n11. Search for a Route\n0. Exit\n";
        cin >> operation;

        switch (operation) {
            case 1: cout << "Intersection ID: "; 
            cin >> id1; cout << "Intersection Name: "; 
            cin >> Iname; Network.AddIntersection(Intersection(id1, Iname)); break;
            case 2: cout << "Intersection ID to update: "; 
            cin >> id1; cout << "New Name: "; 
            cin >> Iname; Network.UpdateIntersection(id1, Iname); break;
            case 3: cout << "Intersection ID to remove: "; 
            cin >> id1; Network.RemoveIntersection(id1); break;
            case 4: cout << "From Intersection ID: "; 
            cin >> id1; cout << "To Intersection ID: "; 
            cin >> id2; cout << "Distance: "; 
            cin >> dist; Network.AddRoute(id1, id2, dist); break;
            case 5: cout << "From Intersection ID: "; 
            cin >> id1; cout << "To Intersection ID: "; 
            cin >> id2; cout << "New Distance: "; 
            cin >> dist; Network.UpdateRoute(id1, id2, dist); break;
            case 6: cout << "From Intersection ID: "; 
            cin >> id1; cout << "To Intersection ID: "; 
            cin >> id2; Network.RemoveRoute(id1, id2); break;
            case 7: cout << "Intersection ID to print neighbors: "; 
            cin >> id1; Network.printNeighbors(id1); break;
            case 8: Network.printTransportNetwork(); break;
            case 9: 
                cout << "Enter current hour (0-23) for congestion prediction: "; cin >> hour;
                cout << "Start Intersection ID: "; cin >> id1; cout << "End Intersection ID: "; cin >> id2; 
                Network.ShortestPath(id1, id2, hour); break;
            case 10: cout << "Intersection ID to sort routes: "; cin >> id1; Network.SortRoutesByDistance(id1); break;
            case 11: cout << "From Intersection ID: "; cin >> id1; cout << "To Intersection ID: "; cin >> id2; Network.SearchRoute(id1, id2); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid option.\n";
        }
    } while (operation != 0);

    return 0;
}
