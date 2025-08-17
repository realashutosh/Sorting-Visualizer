#include <bits/stdc++.h>
#include <chrono>
#include <thread>
using namespace std;

// Simple ANSI-terminal sorting visualizer
// Works on Linux/macOS terminals that support ANSI escape codes.
// Compile: g++ -std=c++17 -O2 -pthread sort_visualizer.cpp -o sortviz
// Run:     ./sortviz

// ============ ANSI helpers ============
namespace ansi {
    const string CLEAR = "\033[2J";
    const string HIDE  = "\033[?25l";
    const string SHOW  = "\033[?25h";
    string cursor(int r, int c){ return "\033[" + to_string(r) + ";" + to_string(c) + 'H'; }
    string color(int c){ return "\033[38;5;"+to_string(c)+"m"; }
    const string RESET = "\033[0m";
}

struct VizConfig {
    int n = 50;                 // number of elements
    int delay_ms = 20;          // delay between frames
    bool show_values = false;   // print numbers under bars
    int max_height = 20;        // max bar height in rows
};

static VizConfig CFG;
static vector<int> A;  // the array

// Map value in [1..max] to height in [1..CFG.max_height]
int toHeight(int val, int maxv){
    return max(1, (val * CFG.max_height) / max(1, maxv));
}

void drawArray(const vector<int>& arr, int i=-1, int j=-1, int k=-1, const string& status=""){
    int n = (int)arr.size();
    int maxv = *max_element(arr.begin(), arr.end());

    // Build a grid of characters (rows x cols)
    int rows = CFG.max_height;
    int cols = n;

    // Clear and position
    cout << ansi::CLEAR << ansi::cursor(1,1) << ansi::HIDE;

    // Title / status
    cout << "Sorting Visualizer (n=" << n << ", delay=" << CFG.delay_ms << "ms)\n";
    if(!status.empty()) cout << status << "\n"; else cout << "\n";

    // Draw bars from top row to bottom row
    for(int r=rows; r>=1; --r){
        for(int c=0; c<cols; ++c){
            int h = toHeight(arr[c], maxv);
            bool filled = (h >= r);
            int color = 244; // default gray
            if(c==i) color = 196; // red
            else if(c==j) color = 45; // cyan
            else if(c==k) color = 226; // yellow
            if(filled){
                cout << ansi::color(color) << "█" << ansi::RESET;
            } else {
                cout << " ";
            }
        }
        cout << "\n";
    }

    if(CFG.show_values){
        for(int c=0;c<cols;++c){
            if(arr[c] < 10) cout << arr[c];
            else cout << (arr[c]%10); // single-digit to keep columns compact
        }
        cout << "\n";
    }

    cout.flush();
    this_thread::sleep_for(chrono::milliseconds(CFG.delay_ms));
}

// Utility to shuffle array
void shuffleArray(vector<int>& a){
    random_device rd; mt19937 rng(rd());
    shuffle(a.begin(), a.end(), rng);
}

// ================== Sorting algorithms with visualization ==================

void bubbleSort(vector<int> a){
    int n=a.size();
    for(int i=0;i<n;i++){
        bool swapped=false;
        for(int j=0;j<n-1-i;j++){
            drawArray(a, j, j+1, -1, "Bubble Sort: comparing j and j+1");
            if(a[j] > a[j+1]){
                swap(a[j], a[j+1]);
                swapped=true;
                drawArray(a, j, j+1, -1, "Bubble Sort: swapped");
            }
        }
        if(!swapped) break;
    }
    drawArray(a, -1, -1, -1, "Bubble Sort: done");
}

void insertionSort(vector<int> a){
    int n=a.size();
    for(int i=1;i<n;i++){
        int key=a[i];
        int j=i-1;
        while(j>=0 && a[j]>key){
            a[j+1]=a[j];
            drawArray(a, j, j+1, i, "Insertion Sort: shifting");
            j--;
        }
        a[j+1]=key;
        drawArray(a, j+1, -1, i, "Insertion Sort: placed key");
    }
    drawArray(a, -1, -1, -1, "Insertion Sort: done");
}

void selectionSort(vector<int> a){
    int n=a.size();
    for(int i=0;i<n;i++){
        int minIdx=i;
        for(int j=i+1;j<n;j++){
            drawArray(a, minIdx, j, i, "Selection Sort: scanning for min");
            if(a[j]<a[minIdx]) minIdx=j;
        }
        swap(a[i], a[minIdx]);
        drawArray(a, i, minIdx, -1, "Selection Sort: swapped min into place");
    }
    drawArray(a, -1, -1, -1, "Selection Sort: done");
}

void mergeViz(vector<int>& a, int l, int m, int r){
    vector<int> left(a.begin()+l, a.begin()+m+1);
    vector<int> right(a.begin()+m+1, a.begin()+r+1);
    int i=0,j=0,k=l;
    while(i<(int)left.size() && j<(int)right.size()){
        drawArray(a, k, l+i, m+1+j, "Merge Sort: merging");
        if(left[i]<=right[j]) a[k++]=left[i++];
        else a[k++]=right[j++];
    }
    while(i<(int)left.size()){ a[k++]=left[i++]; drawArray(a, k-1, -1, -1, "Merge Sort: copying left"); }
    while(j<(int)right.size()){ a[k++]=right[j++]; drawArray(a, k-1, -1, -1, "Merge Sort: copying right"); }
}

void mergeSortRec(vector<int>& a, int l, int r){
    if(l>=r) return;
    int m=(l+r)/2;
    mergeSortRec(a,l,m);
    mergeSortRec(a,m+1,r);
    mergeViz(a,l,m,r);
}

void mergeSort(vector<int> a){
    mergeSortRec(a,0,(int)a.size()-1);
    drawArray(a,-1,-1,-1,"Merge Sort: done");
}

int partitionViz(vector<int>& a, int lo, int hi){
    int pivot=a[hi];
    int i=lo;
    for(int j=lo;j<hi;j++){
        drawArray(a, j, hi, i, "Quick Sort: partitioning");
        if(a[j] < pivot){
            swap(a[i], a[j]);
            drawArray(a, j, hi, i, "Quick Sort: swap < pivot");
            i++;
        }
    }
    swap(a[i], a[hi]);
    drawArray(a, i, hi, -1, "Quick Sort: pivot placed");
    return i;
}

void quickSortRec(vector<int>& a, int lo, int hi){
    if(lo>=hi) return;
    int p = partitionViz(a, lo, hi);
    quickSortRec(a, lo, p-1);
    quickSortRec(a, p+1, hi);
}

void quickSort(vector<int> a){
    quickSortRec(a,0,(int)a.size()-1);
    drawArray(a,-1,-1,-1,"Quick Sort: done");
}

void heapifyViz(vector<int>& a, int n, int i){
    int largest=i;
    int l=2*i+1, r=2*i+2;
    if(l<n && a[l]>a[largest]) largest=l;
    if(r<n && a[r]>a[largest]) largest=r;
    if(largest!=i){
        swap(a[i], a[largest]);
        drawArray(a, i, largest, n-1, "Heap Sort: heapify");
        heapifyViz(a,n,largest);
    }
}

void heapSort(vector<int> a){
    int n=a.size();
    for(int i=n/2-1;i>=0;i--) heapifyViz(a,n,i);
    for(int i=n-1;i>0;i--){
        swap(a[0], a[i]);
        drawArray(a, 0, i, -1, "Heap Sort: extract max");
        heapifyViz(a,i,0);
    }
    drawArray(a,-1,-1,-1,"Heap Sort: done");
}

// ================== Menu / Driver ==================

void generateArray(int n){
    A.resize(n);
    for(int i=0;i<n;i++) A[i] = i+1;
    shuffleArray(A);
}

void printMenu(){
    cout << ansi::SHOW; // ensure cursor visible for input
    cout << "\n";
    cout << "+----------------------------------------------+\n";
    cout << "|           Sorting Visualizer (ANSI)          |\n";
    cout << "+----------------------------------------------+\n";
    cout << " 1) Bubble Sort\n";
    cout << " 2) Insertion Sort\n";
    cout << " 3) Selection Sort\n";
    cout << " 4) Merge Sort\n";
    cout << " 5) Quick Sort\n";
    cout << " 6) Heap Sort\n";
    cout << " s) Shuffle array\n";
    cout << " v) Toggle show values (current: " << (CFG.show_values?"ON":"OFF") << ")\n";
    cout << " +) Increase delay (now: " << CFG.delay_ms << "ms)\n";
    cout << " -) Decrease delay (now: " << CFG.delay_ms << "ms)\n";
    cout << " n) Change N (now: " << CFG.n << ")\n";
    cout << " q) Quit\n";
    cout << "Select: ";
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Initial setup
    cout << ansi::CLEAR << ansi::cursor(1,1);

    // Basic interactive loop
    generateArray(CFG.n);
    drawArray(A, -1, -1, -1, "Ready. Open menu in console below.");

    while(true){
        printMenu();
        string choice; if(!(cin>>choice)) break;
        if(choice=="1") bubbleSort(A);
        else if(choice=="2") insertionSort(A);
        else if(choice=="3") selectionSort(A);
        else if(choice=="4") mergeSort(A);
        else if(choice=="5") quickSort(A);
        else if(choice=="6") heapSort(A);
        else if(choice=="s" || choice=="S"){ shuffleArray(A); drawArray(A,-1,-1,-1,"Shuffled."); }
        else if(choice=="v" || choice=="V"){ CFG.show_values = !CFG.show_values; drawArray(A,-1,-1,-1, string("Show values ") + (CFG.show_values?"ON":"OFF")); }
        else if(choice=="+") { CFG.delay_ms = min(1000, CFG.delay_ms+10); drawArray(A,-1,-1,-1, "Delay increased."); }
        else if(choice=="-") { CFG.delay_ms = max(0, CFG.delay_ms-10); drawArray(A,-1,-1,-1, "Delay decreased."); }
        else if(choice=="n" || choice=="N"){ 
            cout << "Enter new N (5..200): ";
            int nn; if(cin>>nn){ nn=max(5,min(200,nn)); CFG.n=nn; generateArray(CFG.n); drawArray(A,-1,-1,-1,"Size changed and shuffled."); }
        }
        else if(choice=="q" || choice=="Q"){
            cout << ansi::SHOW << ansi::RESET << "\nBye!\n";
            break;
        }
        else {
            drawArray(A,-1,-1,-1,"Unknown option.");
        }
    }

    cout << ansi::SHOW << ansi::RESET;
    return 0;
}
