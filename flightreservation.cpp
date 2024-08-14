#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <cfloat>
using namespace std;

struct Airport {
    string code;
    string name;
};

struct Flight {
    string flightNumber;
    string source;
    string destination;
    int distance;
    int availableSeats;
    double price;
    vector<bool> seatMap;
};

struct Reservation {
    string flightNumber;
    string passengerName;
    int seatNumber;
};

class FlightReservationSystem {
private:
    unordered_map<string, Airport> airports;
    unordered_map<string, Flight> flights;
    vector<Reservation> reservations;

public:
    void addAirport(string code, string name) {
        airports[code] = {code, name};
    }

    void addFlight(string flightNumber, string source, string destination, int distance, int availableSeats, double price) {
        flights[flightNumber] = {flightNumber, source, destination, distance, availableSeats, price, vector<bool>(availableSeats, false)};
    }

    void searchFlights(string source, string destination) {
    cout << "Available Flights from " << source << " to " << destination << ":" << endl;
    
    for (const auto& pair : flights) {
        const Flight& flight = pair.second;

        // direct flights 
        if (flight.source == source && flight.destination == destination) {
            cout << "Flight: " << flight.flightNumber << ", Price: $" << flight.price << ", Seats Available: " << flight.availableSeats << endl;
        }

        // indirect flights
        else if (flight.source == source) {
            for (const auto& nextFlightPair : flights) {
                const Flight& nextFlight = nextFlightPair.second;
                if (nextFlight.source == flight.destination && nextFlight.destination == destination) {
                    cout << "Indirect Flights: " << endl;
                    cout << "Flight 1: " << flight.flightNumber << ", Price: $" << flight.price << ", Seats Available: " << flight.availableSeats << endl;
                    cout << "Flight 2: " << nextFlight.flightNumber << ", Price: $" << nextFlight.price << ", Seats Available: " << nextFlight.availableSeats << endl;
                }
            }
        }
    }
}

    void viewReservations(string passengerName) {
        cout << "Reservations for " << passengerName << ":" << endl;
        for (const Reservation& reservation : reservations) {
            if (reservation.passengerName == passengerName) {
                const Flight& flight = flights[reservation.flightNumber];
                cout << "Flight: " << flight.flightNumber << ", Seat: " << reservation.seatNumber << endl;
            }
        }
    }

    void makeReservation(string flightNumber, string passengerName, int seatNumber) {
        if (flights.find(flightNumber) != flights.end()) {
            Flight& flight = flights[flightNumber];
            if (seatNumber >= 1 && seatNumber <= flight.availableSeats) {
                if (!flight.seatMap[seatNumber - 1]) {
                    flight.availableSeats--;
                    flight.seatMap[seatNumber - 1] = true;
                    reservations.push_back({flightNumber, passengerName, seatNumber});
                    cout << "Reservation successful. Your seat number is " << seatNumber << endl;
                } else {
                    cout << "Seat already reserved. Please choose another seat." << endl;
                }
            } else {
                cout << "Invalid seat number. Please choose a valid seat." << endl;
            }
        } else {
            cout << "Flight not found." << endl;
        }
    }

    void viewFlightDetails(string flightNumber) {
        if (flights.find(flightNumber) != flights.end()) {
            Flight& flight = flights[flightNumber];
            cout << "Flight Number: " << flight.flightNumber << endl;
            cout << "Source: " << flight.source << endl;
            cout << "Destination: " << flight.destination << endl;
            cout << "Distance: " << flight.distance << " miles" << endl;
            cout << "Price: $" << flight.price << endl;
            cout << "Available Seats: " << flight.availableSeats << endl;

            cout << "Seat Map (1: Available, 0: Reserved): ";
            for (bool seat : flight.seatMap) {
                cout << seat << " ";
            }
            cout << endl;
        } else {
            cout << "Flight not found." << endl;
        }
    }

    void cancelReservation(string flightNumber, string passengerName, int seatNumber) {
        auto it = find_if(reservations.begin(), reservations.end(), [&](const Reservation& r) {
            return r.flightNumber == flightNumber && r.passengerName == passengerName && r.seatNumber == seatNumber;
        });

        if (it != reservations.end()) {
            Flight& flight = flights[flightNumber];
            flight.availableSeats++;
            flight.seatMap[seatNumber - 1] = false;
            reservations.erase(it);
            cout << "Reservation canceled successfully." << endl;
        } else {
            cout << "Reservation not found." << endl;
        }
    }
    void displayAvailableAirports() {
        cout << "Available Airports: " << endl;
        for (const auto& pair : airports) {
            const Airport& airport = pair.second;
            cout << "Code: " << airport.code << ", Name: " << airport.name << endl;
        }
    }

    void displayReservedSeats(string flightNumber) {
        if (flights.find(flightNumber) != flights.end()) {
            const Flight& flight = flights[flightNumber];
            cout << "Reserved Seats for Flight " << flightNumber << ": " << endl;
            for (const Reservation& reservation : reservations) {
                if (reservation.flightNumber == flightNumber) {
                    cout << "Passenger: " << reservation.passengerName << ", Seat: " << reservation.seatNumber << endl;
                }
            }
        } else {
            cout << "Flight not found." << endl;
        }
    }
    void comparePrices(string source, string destination) {
    double minPrice = DBL_MAX; // setting initial minPrice to a very large value
    string bestFlightNumber;
        for (const auto& pair : flights) {
            const Flight& flight = pair.second;
            if (flight.source == source && flight.destination == destination && flight.price < minPrice) {
                minPrice = flight.price;
                bestFlightNumber = flight.flightNumber;
            }
        }
        if (bestFlightNumber.empty()) {
            cout << "No flights available between " << source << " and " << destination << "." << endl;
        } else {
            cout << "Best Flight Option - Flight: " << bestFlightNumber << ", Price: $" << minPrice << endl;
        }
    }

    double calculateTotalCost(string flightNumber, int seatCount) {
        if (flights.find(flightNumber) != flights.end()) {
            const Flight& flight = flights[flightNumber];
            return flight.price * seatCount;
        } else {
            return 0.0;
        }
    }

    void displayFlightSchedule() {
        cout << "Flight Schedule: " << endl;
        for (const auto& pair : flights) {
            const Flight& flight = pair.second;
            cout << "Flight: " << flight.flightNumber << ", Source: " << flight.source << ", Destination: " << flight.destination << endl;
        }
    }

    // a feature to find the shortest path between airports (Dijkstra's algorithm)
    void findShortestPath(string source, string destination) {
        unordered_map<string, double> distance;
        unordered_map<string, string> previous;
        priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> pq;

        for (const auto& pair : airports) {
            distance[pair.first] = DBL_MAX;
            previous[pair.first] = "";
        }

        distance[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            string current = pq.top().second;
            pq.pop();

            for (const auto& pair : flights) {
                const Flight& flight = pair.second;
                if (flight.source == current && distance[current] + flight.distance < distance[flight.destination]) {
                    distance[flight.destination] = distance[current] + flight.distance;
                    previous[flight.destination] = current;
                    pq.push({distance[flight.destination], flight.destination});
                }
            }
        }

        vector<string> path;
        string current = destination;
        while (!previous[current].empty()) {
            path.push_back(current);
            current = previous[current];
        }
        path.push_back(current);
        reverse(path.begin(), path.end());

        cout << "Shortest Path from " << source << " to " << destination << ": ";
        for (const string& airport : path) {
            cout << airport << " ";
        }
        cout << "- Distance: " << distance[destination] << " miles" << endl;
    }
};

int main() {
    FlightReservationSystem system;
    
    system.addAirport("DEL", "Indira Gandhi International Airport, Delhi");
    system.addAirport("BOM", "Chhatrapati Shivaji Maharaj International Airport, Mumbai");
    system.addAirport("MAA", "Chennai International Airport, Chennai");
    system.addAirport("BLR", "Kempegowda International Airport, Bangalore");
    system.addAirport("HYD", "Rajiv Gandhi International Airport, Hyderabad");
    system.addAirport("CCU", "Netaji Subhas Chandra Bose International Airport, Kolkata");
    system.addAirport("AMD", "Sardar Vallabhbhai Patel International Airport, Ahmedabad");
    system.addAirport("PNQ", "Pune Airport, Pune");

    system.addFlight("FL789", "DEL", "BOM", 1200, 80, 250.0);
    system.addFlight("FL101", "BOM", "MAA", 800, 60, 180.0);
    system.addFlight("FL999", "DEL", "MAA", 1500, 100, 220.0);
    system.addFlight("FL1010", "DEL", "MAA", 1600, 120, 250.0);
    system.addFlight("FL202", "DEL", "BLR", 1400, 70, 270.0);
    system.addFlight("FL303", "MAA", "HYD", 600, 50, 150.0);
    system.addFlight("FL404", "BLR", "CCU", 1200, 60, 200.0);
    system.addFlight("FL505", "HYD", "AMD", 800, 40, 160.0);
    system.addFlight("FL707", "BLR", "PNQ", 750, 80, 180.0);
    system.addFlight("FL808", "BLR", "PNQ", 700, 60, 160.0);
    system.addFlight("FL606", "CCU", "PNQ", 900, 30, 175.0);
    

    while (true) {
        cout << "1. Available Airports" << endl;
        cout << "2. Display Flight Schedule" << endl;
        cout << "3. Search Flights" << endl;
        cout << "4. Find Shortest Path" << endl;
        cout << "5. Price comparison "<< endl;
        cout << "6. Make a Reservation" << endl;
        cout << "7. Calculate Total Cost" << endl;
        cout << "8. View Reservations" << endl;
        cout << "9. View Flight Details" << endl;
        cout << "10. Cancel Reservation" << endl;
        cout << "11. Reserved Seats for a Flight" << endl;
        cout << "12. Exit" << endl;
        int choice;
        cin >> choice;
        cin.ignore();

        
        if (choice == 1) {
            system.displayAvailableAirports();
        }
        else if (choice == 2) {
            system.displayFlightSchedule();
        }
        else if (choice == 3) {
            string source, destination;
            cout << "Enter source airport: ";
            getline(cin, source);
            cout << "Enter destination airport: ";
            getline(cin, destination);
            system.searchFlights(source, destination);
        }
        else if (choice == 4) {
            string source, destination;
            cout << "Enter source airport: ";
            getline(cin, source);
            cout << "Enter destination airport: ";
            getline(cin, destination);
            system.findShortestPath(source, destination);
        }
        else if (choice == 5) {
            string source, destination;
            cout << "Enter source airport: ";
            getline(cin, source);
            cout << "Enter destination airport: ";
            getline(cin, destination);
            system.comparePrices(source, destination);  
        }
        else if (choice == 6) {
            string flightNumber, passengerName;
            int seatNumber;
            cout << "Enter flight number: ";
            getline(cin, flightNumber);
            cout << "Enter passenger name: ";
            getline(cin, passengerName);
            cout << "Enter seat number: ";
            cin >> seatNumber;
            cin.ignore();
            system.makeReservation(flightNumber, passengerName, seatNumber);
        }
        else if (choice == 7) {
            string flightNumber;
            int seatCount;
            cout << "Enter flight number: ";
            getline(cin, flightNumber);
            cout << "Enter the number of seats: ";
            cin >> seatCount;
            cin.ignore();
            double totalCost = system.calculateTotalCost(flightNumber, seatCount);
            cout << "Total Cost: $" << totalCost << endl;
        } 
        else if (choice == 8) {
            string passengerName;
            cout << "Enter passenger name: ";
            getline(cin, passengerName);
            system.viewReservations(passengerName);
        } 
        else if (choice == 9) {
            string flightNumber;
            cout << "Enter flight number: ";
            getline(cin, flightNumber);
            system.viewFlightDetails(flightNumber);
        } 
        else if (choice == 10) {
            string flightNumber, passengerName;
            int seatNumber;
            cout << "Enter flight number: ";
            getline(cin, flightNumber);
            cout << "Enter passenger name: ";
            getline(cin, passengerName);
            cout << "Enter seat number: ";
            cin >> seatNumber;
            cin.ignore();
            system.cancelReservation(flightNumber, passengerName, seatNumber);
        }
        else if (choice == 11) {
            string flightNumber;
            cout << "Enter flight number: ";
            getline(cin, flightNumber);
            system.displayReservedSeats(flightNumber);
        } 
        else if (choice == 12) {
            break;
        }
    }
    return 0;
}