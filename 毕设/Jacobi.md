`Jacobi` 迭代示例程序
```c
#define M 1024
#define N 1024
#define TIMES 10000
float **grid;
float scratch[M][N];

int boundary_value = 0; // 零边界处理
int main() {

Tmk_startup();

if (Tmk_proc_id == 0)
{
   grid = Tmk_malloc(M * N * sizeof(float));
   // initialize grid;
}

Tmk_barrier(0);
length = M / Tmk_nprocs;
begin = length * Tmk_proc_id;
end = length * (Tmk_proc_id + 1);
for (int time = 0; time < TIMES; time++) {
{
/*
    for (i = begin; i < end; i++)
        for (j = 0; j < N; j++)
            scratch[i][j] = (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]) / 4;
*/

for (i = begin; i < end; i++) {
    for (j = 0; j < N; j++) {
        if (i == 0) {
            // 顶部行的处理
            scratch[i][j] = (boundary_value + grid[i + 1][j] + 
                             (j > 0 ? grid[i][j - 1] : boundary_value) + 
                             (j < N - 1 ? grid[i][j + 1] : boundary_value)) / 4;
        } else if (i == M - 1) {
            // 底部行的处理
            scratch[i][j] = (grid[i - 1][j] + boundary_value + 
                             (j > 0 ? grid[i][j - 1] : boundary_value) + 
                             (j < N - 1 ? grid[i][j + 1] : boundary_value)) / 4;
        } else {
            // 内部行的处理
            scratch[i][j] = (grid[i - 1][j] + grid[i + 1][j] + 
                             (j > 0 ? grid[i][j - 1] : boundary_value) + 
                             (j < N - 1 ? grid[i][j + 1] : boundary_value)) / 4;
        }
    }
}


    Tmk_barrier(1);
    for (i = begin; i < end; i++）
        for (j = 0; j < N; j++)
            grid[i][j] = scratch[i][j];
    Tmk_barrier(2);
}
}

```