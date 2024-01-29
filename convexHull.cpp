#include <bits/stdc++.h>
#include <cmath>
#include <string>
using namespace std;
// Command to execute code: 
// g++ 22CS60R37-a2.cpp -o a2
// a2 FILENAME

struct Point
{
    int x;
    int y;
};

// function to compare points by x-coordinate
bool compare(Point &p1, Point &p2)
{
    return p1.x < p2.x;
}

double getAngle(Point p)
{
    double angle = 0.0;
    angle = -1 * atan2(p.x, -1 * p.y);
    return angle;
}

// Angle comparison to Sort array of Point in counter clokcwise direction
bool compareAngle(Point &p1, Point &p2)
{
    double angle1 = getAngle(p1);
    double angle2 = getAngle(p2);
    return angle1 < angle2;
}

// Checks whether the point is above the line or below the line
int orientation(struct Point r, struct Point p, struct Point q)
{
    int ans = (p.x - r.x) * (q.y - r.y) - (q.x - r.x) * (p.y - r.y);
    if (ans == 0)
        return 0;
    if (ans > 0)
        return 1;
    return -1;
}

struct Point *convexHull(struct Point arr[], int start, int end, int *count)
{
    // n: number of points
    int n = end - start + 1;
    struct Point *hull = new struct Point[n];
    int k = 0;
    for (int i = start; i <= end; i++)
    {
        while (k >= 2 && orientation(hull[k - 2], hull[k - 1], arr[i]) <= 0)
            k--;
        // adding the point to the convex hull
        hull[k++] = arr[i];
    }
    for (int i = end - 1, t = k + 1; i >= start; i--)
    {
        while (k >= t && orientation(hull[k - 2], hull[k - 1], arr[i]) <= 0)
            k--;
        // adding the point to the convex hull
        hull[k++] = arr[i];
    }
    *count = k - 1;
    // returning the convex hull
    return hull;
}

// Merger function to merge two convex hulls
struct Point *mergeHull(struct Point left[], struct Point right[], int start, int mid, int end)
{
    // size of left array
    int lsize = mid - start + 1;
    // size of right array
    int rsize = end - mid;
    int rightMost = 0;
    int leftMost = 0;
    int max = 0;
    int min = INT_MAX;
    for (int i = 1; i < lsize; i++)
    {
        if (left[start + i].x > max)
            rightMost = start + i;
    }
    for (int i = 1; i < rsize; i++)
    {
        if (right[mid + i].x < min)
            leftMost = mid + i;
    }
    // sorting the points in counter clockwise direction in left hull
    sort(left, left + lsize, compareAngle);
    // sorting the points in counter clockwise direction in right hull
    sort(right, right + rsize, compareAngle);
    int indexRightMost = rightMost;
    // fixing the leftmost point in right hull
    int indexLeftMost = leftMost;
    bool check = false;
    // finding upper tangent
    while (!check)
    {
        check = true;
        while (orientation(left[indexRightMost], right[indexLeftMost], right[(indexLeftMost + 1) % rsize]) >= 0)
        {
            indexLeftMost = (indexLeftMost + 1) % rsize;
        }
        while (orientation(right[indexLeftMost], left[indexRightMost], left[(indexRightMost + 1) % lsize]) < 0)
        {
            indexRightMost = (indexRightMost + 1) % lsize;
            check = false;
        }
    }
    // upper point
    int upperA = indexRightMost;
    int upperB = indexLeftMost;

    // Again fixing the original leftmost and rightmost point
    indexRightMost = rightMost;
    indexLeftMost = leftMost;
    check = false;

    // finding lower tangent
    while (!check)
    {
        check = true;
        while (orientation(right[indexLeftMost], left[indexRightMost], left[(indexRightMost + 1) % lsize]) >= 0)
        {
            indexRightMost = (indexRightMost + 1) % lsize;
        }
        while (orientation(left[indexRightMost], right[indexLeftMost], right[(indexLeftMost + 1) % rsize]) < 0)
        {
            indexLeftMost = (indexLeftMost + 1) % rsize;
            check = false;
        }
    }
    // lower point
    int lowerA = indexRightMost;
    int lowerB = indexLeftMost;

    // size of the merged hull
    struct Point *hull = new struct Point[lsize + rsize];
    // index to store the merged hull
    int i = upperA;
    // traversing left hull from upperA to lowerA in counter clockwise direction to store boundary points in merged hull
    hull[i] = left[i];
    int track = 1;
    while (i != lowerA)
    {
        track++;
        i = (i + 1) % lsize;
        hull[i] = left[i];
    }
    // traversing right hull from lowerB to upperB in counter clockwise direction to store boundary points in merged hull
    i = lowerB;
    hull[i] = right[i];
    while (i != upperB)
    {
        i = (i + 1) % rsize;
        hull[i] = right[i];
        track++;
    }
    return hull;
}

// Divide step for convex hull algorithm
struct Point *convexhull(struct Point arr[], int start, int end)
{
    // base case
    if (end - start + 1 <= 5)
    {
        int count = 0;
        return convexHull(arr, start, end, &count);
    }
    // divide step
    int mid = (start + end) / 2;
    struct Point *left = convexhull(arr, start, mid);
    struct Point *right = convexhull(arr, mid + 1, end);
    // conquer step
    struct Point *hull = mergeHull(left, right, start, mid, end);
    return hull;
}

void Plot(string fname, int n, int g, struct Point pArray[], int maxX, int minX, int newMaxY, int newminY)
{
    FILE *svgWrtr;
    // filename for first program output
    string f = fname + "_1.svg";
    // Open output file for Plot
    svgWrtr = fopen(f.c_str(), "w");
    // writing SVG
    fprintf(svgWrtr, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    fprintf(svgWrtr, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g);
    fprintf(svgWrtr, "<rect width=\"%d\" height=\"%d\" x=\"%d\" y=\"%d\" style=\"fill:rgb(255,255,255);stroke-width:1;stroke:red;stroke-dasharray:2\" />\n", maxX - minX, newMaxY - newminY, g, g);
    for (int i = 0; i < n; i++)
    {
        fprintf(svgWrtr, "<circle cx=\"%d\" cy=\"%d\" r=\"%d\" stroke=\"black\" stroke-width=\"1\" fill=\"red\" />\n", pArray[i].x + g, pArray[i].y + g, 1);
    }
    fprintf(svgWrtr, "<rect width=\"%d\" height=\"%d\" x=\"%d\" y=\"%d\" style=\"fill:rgb(255,255,255);stroke-width:1;stroke:rgb(0,0,0)\" />\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g, 0, 0);
    fprintf(svgWrtr, "</svg>");
    fclose(svgWrtr);
}

string part2(string fname, int n, int g, struct Point pArray[], int maxX, int minX, int newMaxY, int newminY)
{
    int count = n;
    sort(pArray, pArray + n, compare);
    struct Point curPoint = pArray[0];

    bool visited[n];
    // Initialize visited array with false;
    for (int i = 0; i < n; i++)
        visited[i] = false;

    FILE *svgWrtr;
    // filename for part2
    string f = fname + "_2.svg";
    // open output file for part2
    svgWrtr = fopen(f.c_str(), "w");
    // writing SVG header
    fprintf(svgWrtr, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    fprintf(svgWrtr, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g);
    fprintf(svgWrtr, "<rect width=\"%d\" height=\"%d\" x=\"%d\" y=\"%d\" style=\"fill:rgb(255,255,255);stroke-width:1;stroke:rgb(0,0,0)\" />\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g, 0, 0);
    visited[0] = true;
    string dots = "";
    dots += "<circle cx=\"" + to_string(pArray[0].x + g) + "\" cy=\"" + to_string(pArray[0].y + g) + "\" r=\"1\" stroke=\"black\" stroke-width=\"2\" fill=\"red\" />\n";
    // points: string to store coordinates for polygon
    string points = "<polygon points=\"";
    points = points + to_string(pArray[0].x + g) + "," + to_string(pArray[0].y + g) + " ";

    while (count--)
    {
        for (int i = 1; i < n; i++)
        {
            if (visited[i] == false)
            {
                if (sqrt(pow(pArray[i].x - curPoint.x, 2) + pow(pArray[i].y - curPoint.y, 2)) == g || sqrt(pow(pArray[i].x - curPoint.x, 2) + pow(pArray[i].y - curPoint.y, 2)) == sqrt(2) * g)
                {
                    visited[i] = true;
                    curPoint = pArray[i];
                    dots += "<circle cx=\"" + to_string(pArray[i].x + g) + "\" cy=\"" + to_string(pArray[i].y + g) + "\" r=\"1\" stroke=\"black\" stroke-width=\"2\" fill=\"red\" />\n";
                    points = points + to_string(pArray[i].x + g) + "," + to_string(pArray[i].y + g) + " ";
                    break;
                }
            }
        }
    }
    points += "\" fill=\"yellow\" stroke=\"black\" fillRule= 'nonzero' />\n";
    fprintf(svgWrtr, "%s", points.c_str());
    fprintf(svgWrtr, "%s", dots.c_str());
    fprintf(svgWrtr, "</svg>");
    fclose(svgWrtr);
    return points;
}

void Hull(string fname, int n, int g, struct Point pArray[], int maxX, int minX, int newMaxY, int newminY, string points)
{
    FILE *svgWrtr;
    // filename for Hull
    string f = fname + "_3.svg";
    // open output file for Hull
    svgWrtr = fopen(f.c_str(), "w");
    // writing SVG header
    fprintf(svgWrtr, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    fprintf(svgWrtr, "<svg width=\"%d\" height=\"%d\" xmlns=\"http://www.w3.org/2000/svg\">\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g);
    fprintf(svgWrtr, "<rect width=\"%d\" height=\"%d\" x=\"%d\" y=\"%d\" style=\"fill:rgb(255,255,255);stroke-width:1;stroke:rgb(0,0,0)\" />\n", maxX - minX + 2 * g, newMaxY - newminY + 2 * g, 0, 0);
    int count = 0;
    sort(pArray, pArray + n, compare);
    struct Point *cvh = convexHull(pArray, 0, n - 1, &count);
    // struct Point *cvh = convexhull(pArray, 0, n - 1);
    string hullPoints = "";
    string hullPolygon = "";
    // drawing convex hull
    for (int i = 0; i < count; i++)
    {
        // circle to show hull vertices
        hullPoints += "<circle cx=\"" + to_string(cvh[i].x + g) + "\" cy=\"" + to_string(cvh[i].y + g) + "\" r=\"1\" stroke=\"black\" stroke-width=\"2\" fill=\"red\" />\n";
        // appending hull points for drawing hull polygon
        hullPolygon += to_string(cvh[i].x + g) + "," + to_string(cvh[i].y + g) + " ";
    }
    hullPolygon = "<polygon points=\"" + hullPolygon + "\" fill=\"cyan\" stroke=\"black\" fillRule= 'nonzero' />\n";
    fprintf(svgWrtr, "%s", hullPolygon.c_str());
    fprintf(svgWrtr, hullPoints.c_str());
    fprintf(svgWrtr, "%s", points.c_str());
    fprintf(svgWrtr, "</svg>");
    fclose(svgWrtr);
}

int main(int argc, char *argv[])
{
    // pointer to read file
    FILE *fptr;
    // pointer to write output to file
    FILE *svgWrtr;
    // Open input file
    fptr = fopen(argv[1], "r");
    if (fptr == NULL)
    {
        cout << "Can not open the file";
        return 0;
    }

    int g;
    int n;
    // Read n and g
    fscanf(fptr, "%d", &g);
    fscanf(fptr, "%d", &n);

    cout << "g:" << g << " n:" << n << endl;

    int x, y;
    // Array of points
    struct Point pArray[n];

    int maxY = 0;
    int minY = 0;
    int minX = INT_MAX;
    int maxX = INT_MIN;
    // Scan the points from file
    for (int i = 0; i < n; i++)
    {
        fscanf(fptr, "%d %d", &x, &y);
        if (y > maxY)
        {
            maxY = y;
        }
        if (y < minY)
        {
            minY = y;
        }
        if (x < minX)
        {
            minX = x;
        }
        if (x > maxX)
        {
            maxX = x;
        }
        pArray[i].x = x;
        pArray[i].y = y;
    }
    // new minimum and maximum y values after adding g
    int newMaxY = 0;
    int newminY = INT_MAX;
    // new minimum and maximum x values and their indices
    int minXIndex = 0;
    int minXValue = INT_MAX;
    int maxXIndex = 0;
    int maxXValue = INT_MIN;

    for (int i = 0; i < n; i++)
    {
        // subtracting maxY from all y value
        pArray[i].y = maxY - pArray[i].y;
        // Normalizing x coords to shift to origin
        pArray[i].x = pArray[i].x - minX;
        if (pArray[i].y > newMaxY)
        {
            newMaxY = pArray[i].y;
        }
        if (pArray[i].y < newminY)
        {
            newminY = pArray[i].y;
        }
    }

    // find minXIndex and maxXIndex
    for (int i = 0; i < n; i++)
    {
        if (pArray[i].x < minXValue)
        {
            minXValue = pArray[i].x;
            minXIndex = i;
        }
        if (pArray[i].x > maxXValue)
        {
            maxXValue = pArray[i].x;
            maxXIndex = i;
        }
    }
    string fname = "";
    for (int i = 0; i < strlen(argv[1]) - 4; i++)
    {
        fname += argv[1][i];
    }

    Plot(fname, n, g, pArray, maxX, minX, newMaxY, newminY);
    string points = part2(fname, n, g, pArray, maxX, minX, newMaxY, newminY);
    Hull(fname, n, g, pArray, maxX, minX, newMaxY, newminY, points);
}

