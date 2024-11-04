#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <string>

struct City {
    int id;
    double x, y;
};

double euclideanDistance(const City& a, const City& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double calculateTourCost(const std::vector<City>& cities, const std::vector<int>& tour) {
    double totalCost = 0;
    for (size_t i = 0; i < tour.size() - 1; ++i) {
        totalCost += euclideanDistance(cities[tour[i]], cities[tour[i + 1]]);
    }
    totalCost += euclideanDistance(cities[tour.back()], cities[tour[0]]);
    return totalCost;
}

std::vector<int> getNeighbor(const std::vector<int>& tour) {
    std::vector<int> neighbor = tour;
    int i = rand() % tour.size();
    int j = rand() % tour.size();
    std::swap(neighbor[i], neighbor[j]);
    return neighbor;
}

std::vector<City> loadCities(const std::string& filename) {
    std::vector<City> cities;
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return cities;
    }
    std::string line;
    while (std::getline(file, line)) {
        City city;
        if (sscanf(line.c_str(), "%d %lf %lf", &city.id, &city.x, &city.y) == 3) {
            cities.push_back(city);
        }
    }
    return cities;
}

std::vector<int> initializeTour(int numCities) {
    std::vector<int> tour(numCities);
    for (int i = 0; i < numCities; ++i) tour[i] = i;
    std::random_shuffle(tour.begin(), tour.end());
    return tour;
}

std::vector<int> simulatedAnnealing(const std::vector<City>& cities, int maxIterations) {
    double temperature = 10000.0;
    double coolingRate = 0.999;
    int numCities = cities.size();

    std::vector<int> currentTour = initializeTour(numCities);
    double currentCost = calculateTourCost(cities, currentTour);
    std::vector<int> bestTour = currentTour;
    double bestCost = currentCost;

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        std::vector<int> newTour = getNeighbor(currentTour);
        double newCost = calculateTourCost(cities, newTour);

        if (newCost < currentCost || (exp((currentCost - newCost) / temperature) > ((double) rand() / RAND_MAX))) {
            currentTour = newTour;
            currentCost = newCost;
        }

        if (currentCost < bestCost) {
            bestTour = currentTour;
            bestCost = currentCost;
        }

        temperature *= coolingRate;
    }

    std::cout << "Final achieved cost: " << bestCost << std::endl;
    std::cout << "Solution: ";
    for (int city : bestTour) std::cout << city << " ";
    std::cout << std::endl;

    return bestTour;
}

int main() {
    srand(time(0));
    std::string filename = "xqf131.tsp"; // Change to your actual file path
    int maxIterations = 10000;

    std::vector<City> cities = loadCities(filename);
    if (cities.empty()) {
        std::cerr << "Failed to load cities from file." << std::endl;
        return 1;
    }

    simulatedAnnealing(cities, maxIterations);

    return 0;
}