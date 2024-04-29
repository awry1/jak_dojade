#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#define MAXINT 2147483647

typedef struct letter
{
        struct letter* prev;
        struct letter* next;
        char letter;
} String;
typedef struct queue
{
        int x, y;

        struct queue* next;
}Queue;
typedef struct path
{
        int destination, time;

        struct path* next;
}Path;
typedef struct city
{
        int index, x, y;
        String* nameHead;
        String* nameTail;
        Path* paths;

        struct city* next;
}City;

String* createString()
{
        String* head = (String*)malloc(sizeof(String));
        String* tail = (String*)malloc(sizeof(String));
        head->prev = NULL;
        head->next = tail;
        head->letter = NULL;
        tail->prev = head;
        tail->next = NULL;
        tail->letter = NULL;
        return head;
}
void addToString(String* tail, char letter)
{
        String* newNode = (String*)malloc(sizeof(String));
        newNode->letter = letter;

        (tail->prev)->next = newNode;
        newNode->prev = tail->prev;
        newNode->next = tail;
        tail->prev = newNode;
}
void clearString(String* head, String* tail)
{
        String* tmp1, * tmp2;
        tmp1 = head->next;
        while (tmp1 != tail)
        {
                tmp2 = tmp1->next;
                free(tmp1);
                tmp1 = tmp2;
        }
        head->next = tail;
        tail->prev = head;
}
void freeString(String* head, String* tail)
{
        if (head != NULL && tail != NULL)
        {
                clearString(head, tail);
                free(head);
                free(tail);
        }
}
void printString(String* head, String* tail)
{
        String* tmp = head->next;
        while (tmp != tail)
        {
                printf("%c", tmp->letter);
                tmp = tmp->next;
        }
}
bool stringCompare(String* head1, String* tail1, String* head2, String* tail2)
{
        String* tmp1 = head1->next;
        String* tmp2 = head2->next;
        while (tmp1 != tail1 && tmp2 != tail2)
        {
                if (tmp1->letter != tmp2->letter)
                        return false;
                tmp1 = tmp1->next;
                tmp2 = tmp2->next;
        }
        return (tmp1 == tail1 && tmp2 == tail2);
}

Queue* newQueue(int x, int y)
{
        Queue* node = (Queue*)malloc(sizeof(Path));
        node->x = x;
        node->y = y;
        node->next = NULL;
        return node;
}
void push(Queue* head, int x, int y)
{
        Queue* node = newQueue(x, y);
        Queue* tmp = head;
        while (tmp->next != NULL)
                tmp = tmp->next;
        tmp->next = node;
}
void priorityPush(Queue* head, int x, int y)
{
        Queue* node = newQueue(x, y);
        Queue* tmp = head;
        while (tmp->next != NULL && tmp->next->y < y)
                tmp = tmp->next;
        node->next = tmp->next;
        tmp->next = node;
}
void pop(Queue* head)
{
        Queue* node = NULL;
        if (head->next != NULL)
        {
                node = head->next;
                head->next = node->next;
        }
        free(node);
}
Queue* first(Queue* head)
{
        Queue* node = NULL;
        if (head->next != NULL)
                node = head->next;
        return node;
}
bool isEmpty(Queue* head)
{
        if (head->next == NULL)
                return true;
        else
                return false;
}

Path* newPath(int destination, int time)
{
        Path* node = (Path*)malloc(sizeof(Path));
        node->destination = destination;
        node->time = time;
        node->next = NULL;
        return node;
}
Path* addPath(Path* head, int destination, int time)
{
        Path* node = newPath(destination, time);
        Path* tmp = head;
        bool duplicate = 0;
        while (tmp->next != NULL && duplicate != 1)
        {
                tmp = tmp->next;
                if (destination == tmp->destination)
                {
                        duplicate = 1;
                        if (time < tmp->time)
                                tmp->time = time;
                }
        }
        if (duplicate == 0)
                tmp->next = node;
        return node;
}
Path* findNeighbours(City* cityList, int index)
{
        Path* neighbours = NULL;
        City* tmp = cityList;
        while (tmp->next != NULL && tmp->next->index != index)
                tmp = tmp->next;
        if (tmp->next != NULL)
                neighbours = tmp->next->paths;
        return neighbours;
}
void freePathList(Path* pathList)
{
        if (pathList != NULL)
        {
                if (pathList->next != NULL)
                {
                        Path* pretmp = pathList;
                        Path* tmp = pretmp->next;
                        while (tmp->next != NULL)
                        {
                                free(pretmp);
                                pretmp = tmp;
                                tmp = tmp->next;
                        }
                        free(pretmp);
                        free(tmp);
                }
                else
                        free(pathList);
        }
}

City* newList()
{
        City* head = (City*)malloc(sizeof(City));
        head->index = NULL;
        head->x = NULL;
        head->y = NULL;
        head->nameHead = NULL;
        head->nameTail = NULL;
        head->paths = NULL;
        head->next = NULL;
        return head;
}
City* addCity(City* head, int x, int y)
{
        static int id = 0;
        City* node = (City*)malloc(sizeof(City));
        node->index = id;
        node->x = x;
        node->y = y;
        node->nameHead = createString();
        node->nameTail = node->nameHead->next;
        node->paths = newPath(NULL, NULL);

        node->next = head->next;
        head->next = node;

        id++;
        return node;
}
City* findCityName(City* cityList, String* nameHead, String* nameTail)
{
        City* tmp = cityList;
        while (tmp->next != NULL)
        {
                tmp = tmp->next;
                if(stringCompare(tmp->nameHead, tmp->nameTail, nameHead, nameTail) == 1)
                        break;
        }
        if (tmp == cityList)
                return NULL;
        else
                return tmp;
}
City* findCityPosition(City* cityList, int x, int y)
{
        City* tmp = cityList;
        while (tmp->next != NULL)
        {
                tmp = tmp->next;
                if(x == tmp->x && y == tmp->y)
                        break;
        }
        if (tmp == cityList)
                return NULL;
        else
                return tmp;
}
City* findCityIndex(City* cityList, int index)
{
        City* tmp = cityList;
        while (tmp->next != NULL)
        {
                tmp = tmp->next;
                if (index == tmp->index)
                        break;
        }
        if (tmp == cityList)
                return NULL;
        else
                return tmp;
}
int countCities(City* cityList)
{
        int count = 0;
        City* tmp = cityList;
        while (tmp->next != NULL)
        {
                tmp = tmp->next;
                count++;
        }
        return count;
}
void freeCityList(City* cityList)
{
        if (cityList->next != NULL)
        {
                City* pretmp = cityList;
                City* tmp = pretmp->next;
                while (tmp->next != NULL)
                {
                        freePathList(pretmp->paths);
                        freeString(pretmp->nameHead, pretmp->nameTail);
                        free(pretmp);
                        pretmp = tmp;
                        tmp = tmp->next;
                }
                freePathList(pretmp->paths);
                freePathList(tmp->paths);
                freeString(pretmp->nameHead, pretmp->nameTail);
                freeString(tmp->nameHead, tmp->nameTail);
                free(pretmp);
                free(tmp);
        }
        else
                free(cityList);
}

void readMap(int** map, int sizex, int sizey, City* cityList)
{
        int c;
        for (int i = 0; i < sizey; i++)
        {
                for (int j = 0; j < sizex; j++)
                {
                        do
                                c = getchar();
                        while (c < 32);
                        map[i][j] = c;
                        if (map[i][j] == '*')
                                addCity(cityList, j, i);
                }
        }
}
bool isLetterORNUMBER(int** map, int x, int y)
{
        if ((map[y][x] >= 'A' && map[y][x] <= 'Z') || (map[y][x] >= '0' && map[y][x] <= '9'))
                return true;
        else
                return false;
}
void appendCityNames(int** map, int sizex, int sizey, City* cityList)
{
        City* tmp = cityList;
        while (tmp->next != NULL)
        {
                bool done = 0;
                City* current = tmp->next;
                for (int i = -1; i < 2 && done != 1; i++)
                {
                        for (int j = -1; j < 2 && done != 1; j++)
                        {
                                if (i == 0 && j == 0)
                                        continue;

                                int y = current->y + i;
                                int x = current->x + j;
                                if (x >= 0 && x < sizex && y >= 0 && y < sizey)
                                {
                                        if (isLetterORNUMBER(map, x, y))
                                        {
                                                while (x - 1 > 0 && isLetterORNUMBER(map, x - 1, y))
                                                        x--;
                                                addToString(current->nameTail, (char)map[y][x]);
                                                while (x + 1 < sizex && isLetterORNUMBER(map, x + 1, y))
                                                {
                                                        x++;
                                                        addToString(current->nameTail, (char)map[y][x]);
                                                }
                                                done = 1;
                                        }
                                }
                        }
                }
                tmp = tmp->next;
        }
}

void enqueueNeighbours(int sizex, int sizey, bool** visited, Queue* q, int x, int y, int& nodesNextLayer)
{
        int vx[4] = { -1, 1, 0, 0 };
        int vy[4] = { 0, 0, 1, -1 };
        for (int i = 0; i < 4; i++)
        {
                int xx = x + vx[i];
                int yy = y + vy[i];
                if (xx >= 0 && xx < sizex && yy >= 0 && yy < sizey)
                {
                        if (visited[yy][xx] == 0)
                        {
                                push(q, xx, yy);
                                visited[yy][xx] = 1;
                                nodesNextLayer++;
                        }
                }
        }
}
void bfs(int** map, int sizex, int sizey, City* cityList, City* city)
{
        // https://youtu.be/KiCBXu4P-2Y
        int x = city->x;
        int y = city->y;
        Queue* q = newQueue(NULL, NULL);
        int length = 0;
        int nodesCurrentLayer = 1;
        int nodesNextLayer = 0;
        bool** visited = new bool* [sizey];
        for (int i = 0; i < sizey; i++)
        {
                visited[i] = new bool[sizex];
                for (int j = 0; j < sizex; j++)
                {
                        if (map[i][j] == '#' || map[i][j] == '*')
                                visited[i][j] = 0;
                        else
                                visited[i][j] = 1;
                }
        }
        push(q, x, y);
        visited[y][x] = 1;
        enqueueNeighbours(sizex, sizey, visited, q, x, y, nodesNextLayer);
        while (!isEmpty(q))
        {
                x = first(q)->x;
                y = first(q)->y;
                pop(q);
                if (map[y][x] == '*')
                {
                        int id = findCityPosition(cityList, x, y)->index;
                        addPath(city->paths, id, length);
                }
                else
                        enqueueNeighbours(sizex, sizey, visited, q, x, y, nodesNextLayer);
                nodesCurrentLayer--;
                if (nodesCurrentLayer == 0)
                {
                        nodesCurrentLayer = nodesNextLayer;
                        nodesNextLayer = 0;
                        length++;
                }
        }
        free(q);
        for (int i = 0; i < sizey; i++)
                delete[] visited[i];
        delete[] visited;
}
void processMap(int** map, int sizex, int sizey, City* cityList)
{
        City* tmp = cityList;
        while (tmp->next != NULL)
        {
                tmp = tmp->next;
                bfs(map, sizex, sizey, cityList, tmp);
        }
}

void processFlight(City* cityList, String* sourceHead, String* sourceTail, String* destinationHead, String* destinationTail, int time)
{
        City* city1 = findCityName(cityList, sourceHead, sourceTail);
        City* city2 = findCityName(cityList, destinationHead, destinationTail);
        addPath(city1->paths, city2->index, time);
}
void readFlights(City* cityList)
{
        String* sourceHead = createString();
        String* sourceTail = sourceHead->next;
        String* destinationHead = createString();
        String* destinationTail = destinationHead->next;
        int flights = 0, c = getchar();
        do
        {
                c = getchar();
                if (c >= '0' && c <= '9')
                        flights = flights * 10 + (c - '0');
        } while (c != 10);
        for (int i = 0; i < flights; i++)
        {
                int mode = 0, time = 0;
                bool done = 0;
                while (done != 1)
                {
                        c = getchar();
                        if (c < 32)
                        {
                                if (c == 10)
                                        done = 1;
                                else
                                        continue;
                        }
                        else if (c == 32)
                                mode++;
                        else if (c > 32)
                        {
                                if (mode == 0)
                                        addToString(sourceTail, (char)c);
                                else if (mode == 1)
                                        addToString(destinationTail, (char)c);
                                else if (mode == 2)
                                        time = time * 10 + (c - '0');
                        }
                }
                processFlight(cityList, sourceHead, sourceTail, destinationHead, destinationTail, time);
                clearString(sourceHead, sourceTail);
                clearString(destinationHead, destinationTail);
        }
        freeString(sourceHead, sourceTail);
        freeString(destinationHead, destinationTail);
}

void dijkstra(City* cityList, City* source, City* destination, int* distance, int* previous)
{
        // https://youtu.be/pSqmAO-m7Lk
        int s = source->index;
        int e = destination->index;
        distance[s] = 0;
        Queue* pq = newQueue(NULL, NULL);
        push(pq, s, 0);
        while (!isEmpty(pq))
        {
                int index = first(pq)->x;
                int minValue = first(pq)->y;
                pop(pq);
                if (index == e)
                        break;
                if (distance[index] < minValue)
                        continue;
                Path* neighbours = findNeighbours(cityList, index);
                while (neighbours->next != NULL)
                {
                        neighbours = neighbours->next;
                        if (distance[neighbours->destination] < neighbours->time)
                                continue;
                        int newDistance = distance[index] + neighbours->time;
                        if (newDistance < distance[neighbours->destination])
                        {
                                if(index != s)
                                        previous[neighbours->destination] = index;
                                distance[neighbours->destination] = newDistance;
                                priorityPush(pq, neighbours->destination, newDistance);
                        }
                }
        }
}
void recreatePath(City* destination, int* previous, int* path)
{
        int j = 0;
        int e = destination->index;
        for (int i = e; i != -1; i = previous[i])
        {
                path[j] = i;
                j++;
        }
}
void processQuestion(City* cityList, int cityCount, String* sourceHead, String* sourceTail, String* destinationHead, String* destinationTail, int type)
{
        City* city1 = findCityName(cityList, sourceHead, sourceTail);
        City* city2 = findCityName(cityList, destinationHead, destinationTail);
        int* distance = new int[cityCount];
        for (int i = 0; i < cityCount; i++)
                distance[i] = MAXINT;
        int* previous = new int[cityCount];
        for (int i = 0; i < cityCount; i++)
                previous[i] = -1;
        dijkstra(cityList, city1, city2, distance, previous);
        int* path = new int[cityCount];
        for (int i = 0; i < cityCount; i++)
                path[i] = -1;
        recreatePath(city2, previous, path);
        int time = distance[city2->index];
        printf("%d", time);
        if (type == 1)
        {
                City* tmp;
                for (int i = cityCount - 1; i > 0; i--)
                {
                        if (path[i] != -1)
                        {
                                tmp = findCityIndex(cityList, path[i]);
                                printf(" ");
                                printString(tmp->nameHead, tmp->nameTail);
                        }
                }
        }
        printf("\n");
        delete[] distance;
        delete[] previous;
        delete[] path;
}
void readQuestions(City* cityList)
{
        String* sourceHead = createString();
        String* sourceTail = sourceHead->next;
        String* destinationHead = createString();
        String* destinationTail = destinationHead->next;
        int cityCount = countCities(cityList);
        int questions = 0, c;
        do
        {
                c = getchar();
                if (c >= '0' && c <= '9')
                        questions = questions * 10 + (c - '0');
        } while (c != 10);
        for (int i = 0; i < questions; i++)
        {
                int mode = 0, type = 0;
                bool done = 0;
                while (done != 1)
                {
                        c = getchar();
                        if (c < 32)
                        {
                                if (c == 10)
                                        done = 1;
                                else
                                        continue;
                        }
                        else if (c == 32)
                                mode++;
                        else if (c > 32)
                        {
                                if (mode == 0)
                                        addToString(sourceTail, (char)c);
                                else if (mode == 1)
                                        addToString(destinationTail, (char)c);
                                else if (mode == 2)
                                        type = (c - '0');
                        }
                }
                processQuestion(cityList, cityCount, sourceHead, sourceTail, destinationHead, destinationTail, type);
                clearString(sourceHead, sourceTail);
                clearString(destinationHead, destinationTail);
        }
        freeString(sourceHead, sourceTail);
        freeString(destinationHead, destinationTail);
}

void programEnd(int** map, int sizey, City* cityList)
{
        for (int i = 0; i < sizey; i++)
                delete[] map[i];
        delete[] map;
        freeCityList(cityList);
}

int main()
{
        City* cityList = newList();
        int sizex = 0, sizey = 0, c = 0;
        do
        {
                c = getchar();
                if (c >= '0' && c <= '9')
                        sizex = sizex * 10 + (c - '0');
        } while (c != 32);
        do
        {
                c = getchar();
                if (c >= '0' && c <= '9')
                        sizey = sizey * 10 + (c - '0');
        } while (c != 10);
        int** map = new int* [sizey];
        for (int i = 0; i < sizey; i++)
                map[i] = new int[sizex];

        readMap(map, sizex, sizey, cityList);
        appendCityNames(map, sizex, sizey, cityList);
        processMap(map, sizex, sizey, cityList);
        readFlights(cityList);
        readQuestions(cityList);

        programEnd(map, sizey, cityList);
        return 0;
}