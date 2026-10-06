#include <stdio.h>
#include <string.h>

#define MAX 20
#define INF 9999

char names[MAX][30];
int graph[MAX][MAX];
int dist[MAX];
int parent[MAX];
int visited[MAX];
int n = 0;
int source = -1;
int graphReady = 0;
int computed = 0;

int readInt(void)
{
    int x, c;
    while (scanf("%d", &x) != 1) {
        c = getchar();
        while (c != '\n' && c != EOF)
            c = getchar();
        if (c == EOF)
            return -1;
        printf("Invalid input. Enter a number: ");
    }
    return x;
}

void printDist(int value)
{
    if (value == INF)
        printf("%5s", "INF");
    else
        printf("%5d", value);
}

void enterGraph(void)
{
    int i, j, roads, a, b, w;

    printf("Number of locations (2 to %d): ", MAX);
    n = readInt();
    while (n < 2 || n > MAX) {
        printf("Please enter a value between 2 and %d: ", MAX);
        n = readInt();
    }

    for (i = 0; i < n; i++) {
        printf("Name of location %d: ", i + 1);
        scanf(" %29[^\n]", names[i]);
    }

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            graph[i][j] = (i == j) ? 0 : INF;

    printf("Number of roads: ");
    roads = readInt();
    while (roads < 0 || roads > n * (n - 1) / 2) {
        printf("Roads must be between 0 and %d: ", n * (n - 1) / 2);
        roads = readInt();
    }

    for (i = 0; i < roads; i++) {
        printf("Road %d - enter location no., location no., distance: ", i + 1);
        a = readInt();
        b = readInt();
        w = readInt();
        if (a < 1 || a > n || b < 1 || b > n || a == b) {
            printf("Invalid locations. Re-enter this road.\n");
            i--;
            continue;
        }
        if (w < 0 || w >= INF) {
            printf("Distance must be non-negative and below %d. Re-enter this road.\n", INF);
            i--;
            continue;
        }
        graph[a - 1][b - 1] = w;
        graph[b - 1][a - 1] = w;
    }

    graphReady = 1;
    source = -1;
    computed = 0;
    printf("Campus graph stored.\n");
}

void displayMatrix(void)
{
    int i, j;

    if (!graphReady) {
        printf("Enter the campus graph first (option 1).\n");
        return;
    }

    printf("\nAdjacency Matrix\n");
    printf("%-22s", "");
    for (j = 0; j < n; j++)
        printf("%5d", j + 1);
    printf("\n");

    for (i = 0; i < n; i++) {
        printf("%2d %-19s", i + 1, names[i]);
        for (j = 0; j < n; j++)
            printDist(graph[i][j]);
        printf("\n");
    }
}

void selectSource(void)
{
    int i, choice;

    if (!graphReady) {
        printf("Enter the campus graph first (option 1).\n");
        return;
    }

    printf("\nLocations\n");
    for (i = 0; i < n; i++)
        printf("%d. %s\n", i + 1, names[i]);

    printf("Select source location: ");
    choice = readInt();
    while (choice < 1 || choice > n) {
        printf("Choose a number from 1 to %d: ", n);
        choice = readInt();
    }

    source = choice - 1;
    computed = 0;
    printf("Source set to %s.\n", names[source]);
}

void showStep(int step, int u)
{
    int i;

    printf("Step %d: picked %s (distance %d)\n", step, names[u], dist[u]);
    printf("   dist[]   :");
    for (i = 0; i < n; i++)
        printDist(dist[i]);
    printf("\n   parent[] :");
    for (i = 0; i < n; i++)
        printf("%5d", parent[i] == -1 ? -1 : parent[i] + 1);
    printf("\n");
}

void findShortestDistance(void)
{
    int i, v, u, step, min;

    if (!graphReady) {
        printf("Enter the campus graph first (option 1).\n");
        return;
    }
    if (source == -1) {
        printf("Select a source location first (option 3).\n");
        return;
    }

    for (i = 0; i < n; i++) {
        dist[i] = INF;
        parent[i] = -1;
        visited[i] = 0;
    }
    dist[source] = 0;

    printf("\nDijkstra's Algorithm from %s\n", names[source]);
    printf("Locations are numbered 1 to %d in the order entered.\n\n", n);

    for (step = 1; step <= n; step++) {
        u = -1;
        min = INF;
        for (v = 0; v < n; v++) {
            if (!visited[v] && dist[v] < min) {
                min = dist[v];
                u = v;
            }
        }
        if (u == -1)
            break;

        visited[u] = 1;

        for (v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
            }
        }
        showStep(step, u);
    }

    computed = 1;
    printf("\nShortest distances calculated.\n");
    printf("Time complexity: O(V^2). The outer loop runs V times, and each pass\n");
    printf("scans V vertices to find the minimum and V vertices to relax edges.\n");
}

void printPath(int v)
{
    if (parent[v] != -1) {
        printPath(parent[v]);
        printf(" -> ");
    }
    printf("%s", names[v]);
}

int resultsReady(void)
{
    if (!graphReady) {
        printf("Enter the campus graph first (option 1).\n");
        return 0;
    }
    if (source == -1) {
        printf("Select a source location first (option 3).\n");
        return 0;
    }
    if (!computed) {
        printf("Run Find Shortest Distance first (option 4).\n");
        return 0;
    }
    return 1;
}

void displayPaths(void)
{
    int i;

    if (!resultsReady())
        return;

    printf("\nSource: %s\n", names[source]);
    printf("%-22s %-18s %s\n", "Destination", "Shortest Distance", "Shortest Path");
    for (i = 0; i < n; i++) {
        if (i == source)
            continue;
        printf("%-22s ", names[i]);
        if (dist[i] == INF) {
            printf("%-18s %s\n", "-", "No path");
        } else {
            printf("%-18d ", dist[i]);
            printPath(i);
            printf("\n");
        }
    }
}

void displayAllDistances(void)
{
    int i;

    if (!resultsReady())
        return;

    printf("\nDistance from %s to all locations\n", names[source]);
    for (i = 0; i < n; i++) {
        printf("%2d. %-22s ", i + 1, names[i]);
        if (dist[i] == INF)
            printf("unreachable\n");
        else
            printf("%d\n", dist[i]);
    }
}

int main(void)
{
    int choice;

    do {
        printf("\n===== Campus Shortest Route Finder =====\n");
        printf("1. Enter Campus Graph\n");
        printf("2. Display Adjacency Matrix\n");
        printf("3. Select Source Location\n");
        printf("4. Find Shortest Distance\n");
        printf("5. Display Shortest Paths\n");
        printf("6. Display Distance from Source to All Locations\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        choice = readInt();

        switch (choice) {
        case 1:
            enterGraph();
            break;
        case 2:
            displayMatrix();
            break;
        case 3:
            selectSource();
            break;
        case 4:
            findShortestDistance();
            break;
        case 5:
            displayPaths();
            break;
        case 6:
            displayAllDistances();
            break;
        case 7:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please enter a number from 1 to 7.\n");
        }
    } while (choice != 7 && choice != -1);

    return 0;
}
