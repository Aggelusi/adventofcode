#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    char *city1;
    char *city2;
    unsigned int distance;
} Route;

FILE *open_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Failed to open %s\n", filename);
    }
    return file;
}

unsigned int **initialize_distance_matrix(size_t city_count) {
    unsigned int **distance_matrix = malloc(city_count * sizeof(unsigned int *));
    if (!distance_matrix) {
        fprintf(stderr, "Memory allocation failure\n");
        return NULL;
    }

    for (size_t i = 0; i < city_count; i++) {
        distance_matrix[i] = malloc(city_count * sizeof(unsigned int));
        if (!distance_matrix[i]) {
            fprintf(stderr, "Memory allocation failure\n");
            for (size_t j = 0; j < i; j++) {
                free(distance_matrix[j]);
            }
            free(distance_matrix);
            return NULL;
        }

        for (size_t j = 0; j < city_count; j++) {
            distance_matrix[i][j] = (i == j) ? 0 : 4294967295; // 4294967295 is the maximum value of unsigned int
        }
    }

    return distance_matrix;
}

void fill_distance_matrix(unsigned int **distance_matrix, Route *routes, size_t route_count, char **unique_cities, size_t city_count) {
    for (size_t i = 0; i < route_count; i++) {
        size_t from_city_index = 0, to_city_index = 0;
        for (size_t j = 0; j < city_count; j++) {
            if (strcmp(routes[i].city1, unique_cities[j]) == 0) {
                from_city_index = j;
            }
            if (strcmp(routes[i].city2, unique_cities[j]) == 0) {
                to_city_index = j;
            }
        }
        distance_matrix[from_city_index][to_city_index] = routes[i].distance;
        distance_matrix[to_city_index][from_city_index] = routes[i].distance; // Assuming undirected graph
    }
}

void cleanup_resources(Route *routes, size_t route_count, char **unique_cities, size_t city_count, 
                        unsigned int **distance_matrix, unsigned int **dp, size_t dp_size) {
    // Free routes
    for (size_t i = 0; i < route_count; i++) {
        free(routes[i].city1);
        free(routes[i].city2);
    }
    free(routes);
    for (size_t i = 0; i < city_count; i++) {
        free(distance_matrix[i]);
    }
    free(distance_matrix);
    free(unique_cities);
    if (dp) {
        for (size_t i = 0; i < dp_size; i++) {
            free(dp[i]);
        }
        free(dp);
    }
}


size_t find_city_index(const char *city, char **unique_cities, size_t city_count) {
    for (size_t i = 0; i < city_count; i++) {
        if (strcmp(city, unique_cities[i]) == 0) {
            return i;
        }
    }
    return city_count; // Not found
}

bool is_city_unique(char *city, char **unique_cities, size_t city_count) {
    for (size_t i = 0; i < city_count; i++) {
        if (strcmp(city, unique_cities[i]) == 0) {
            return false; // City already in the list
        }
    }
    return true; // City is unique
}

char **extract_unique_cities(Route *routes, size_t route_count, size_t *city_count) {
    char **unique_cities = NULL;
    size_t count = 0;

    for (size_t i = 0; i < route_count; i++) {
        if (is_city_unique(routes[i].city1, unique_cities, count)) {
            char **temp = realloc(unique_cities, (count + 1) * sizeof(char *));
            if (!temp) {
                fprintf(stderr, "Memory allocation failure\n");
                free(unique_cities);
                return NULL;
            }
            unique_cities = temp;
            unique_cities[count++] = routes[i].city1;
        }
        if (is_city_unique(routes[i].city2, unique_cities, count)) {
            char **temp = realloc(unique_cities, (count + 1) * sizeof(char *));
            if (!temp) {
                fprintf(stderr, "Memory allocation failure\n");
                free(unique_cities);
                return NULL;
            }
            unique_cities = temp;
            unique_cities[count++] = routes[i].city2;
        }
    }

    *city_count = count;
    return unique_cities;
}

Route *parse_routes(FILE *file, size_t *route_count) {
    Route *routes = NULL;
    size_t count = 0;
    char *buffer = NULL;
    size_t buffer_size = 0;

    while (getline(&buffer, &buffer_size, file) != -1) {
        char *newline_pos = strchr(buffer, '\n');
        if (newline_pos) {
            *newline_pos = '\0';
        }

        char *dash_pos = strchr(buffer, '-');
        char *colon_pos = strchr(buffer, ':');
        if (!dash_pos || !colon_pos || dash_pos > colon_pos) {
            fprintf(stderr, "Invalid line format\n");
            free(buffer);
            free(routes);
            return NULL;
        }

        size_t length1 = dash_pos - buffer;
        size_t length2 = colon_pos - dash_pos - 1;

        Route *temp = realloc(routes, (count + 1) * sizeof(Route));
        if (!temp) {
            fprintf(stderr, "Memory allocation failure\n");
            free(buffer);
            free(routes);
            return NULL;
        }

        routes = temp;
        routes[count].city1 = malloc(length1 + 1);
        routes[count].city2 = malloc(length2 + 1);
        if (!routes[count].city1 || !routes[count].city2) {
            fprintf(stderr, "Memory allocation failure\n");
            free(buffer);
            free(routes);
            return NULL;
        }

        strncpy(routes[count].city1, buffer, length1);
        routes[count].city1[length1] = '\0';
        strncpy(routes[count].city2, dash_pos + 1, length2);
        routes[count].city2[length2] = '\0';
        routes[count].distance = (unsigned int)strtoul(colon_pos + 1, NULL, 10);

        count++;
    }

    free(buffer);
    *route_count = count;
    return routes;
}

void held_karp_step_2(unsigned int **distance_matrix, unsigned int **dp, size_t city_count) {
    // Iterate over all cities except the starting city (0)
    for (size_t i = 1; i < city_count; i++) {
        // Subset containing only the start city and city i
        unsigned int subset = (1 << 0) | (1 << i); // Bitmask for subset {0, i}
        dp[subset][i] = distance_matrix[0][i];    // Cost to travel directly from 0 to i
    }
}

unsigned int **initialize_dp_table(size_t city_count) {
    size_t subset_count = 1 << city_count; // Total subsets = 2^city_count
    unsigned int **dp = malloc(subset_count * sizeof(unsigned int *));
    if (!dp) {
        fprintf(stderr, "Memory allocation failure\n");
        return NULL;
    }

    for (size_t i = 0; i < subset_count; i++) {
        dp[i] = malloc(city_count * sizeof(unsigned int));
        if (!dp[i]) {
            fprintf(stderr, "Memory allocation failure\n");
            for (size_t j = 0; j < i; j++) {
                free(dp[j]);
            }
            free(dp);
            return NULL;
        }

        // Initialize all entries to a large value (infinity)
        for (size_t j = 0; j < city_count; j++) {
            dp[i][j] = UINT_MAX;
        }
    }

    return dp;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = open_file(argv[1]);
    if (!file) {
        fprintf(stderr, "Failed to open file");
        return 1;
    }

    size_t route_count = 0;
    Route *routes = parse_routes(file, &route_count);
    fclose(file);
    if (!routes) {
        fprintf(stderr, "Memory allocation failure\n");
        return 1;
    }

    size_t city_count = 0;
    char **unique_cities = extract_unique_cities(routes, route_count, &city_count);
    if (!unique_cities) {
        cleanup_resources(routes, route_count, NULL, 0, NULL, NULL, 0);
        return 1;
    }

    unsigned int **distance_matrix = initialize_distance_matrix(city_count);
    if (!distance_matrix) {
        cleanup_resources(routes, route_count, unique_cities, city_count, NULL, NULL, 0);
        return 1;
    }
    fill_distance_matrix(distance_matrix, routes, route_count, unique_cities, city_count);
    
    unsigned int **dp_table = initialize_dp_table(city_count);
    if (!dp_table) {
        cleanup_resources(routes, route_count, unique_cities, city_count, distance_matrix, NULL, 0);
        return 1;
    }

    held_karp_step_2(distance_matrix, dp_table, city_count);

    
    cleanup_resources(routes, route_count, unique_cities, city_count, distance_matrix, dp_table, 1 << city_count);
    return 0;
}
