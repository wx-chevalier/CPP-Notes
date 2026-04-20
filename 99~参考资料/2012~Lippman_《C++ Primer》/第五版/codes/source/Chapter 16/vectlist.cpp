#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <list>
#include <vector>

int main() {
  using namespace std;
  int n_elem;
  cout << "Enter number of elements: ";
  cin >> n_elem;
  vector<int> vi0(n_elem);
  for (auto pv = vi0.begin(); pv != vi0.end(); pv++)
    *pv = rand();
  vector<int> vi(vi0);

  clock_t start = clock();
  sort(vi.begin(), vi.end());
  clock_t end = clock();
  cout.precision(3);
  cout << (double)(end - start) / CLOCKS_PER_SEC;
  cout << " = elapsed time\n";
  list<int> li(vi.size());
  copy(vi0.begin(), vi0.end(), li.begin());
  list<int> licp(li);

  start = clock();
  li.sort();
  end = clock();
  cout << (double)(end - start) / CLOCKS_PER_SEC;
  cout << " = elapsed time\n";

  start = clock();
  copy(licp.begin(), licp.end(), vi.begin());
  sort(vi.begin(), vi.end());
  copy(vi.begin(), vi.end(), licp.begin());
  end = clock();
  cout << (double)(end - start) / CLOCKS_PER_SEC;
  cout << " = elapsed time\n";

  cout << "done\n";
  // cin.get();
  return 0;
}