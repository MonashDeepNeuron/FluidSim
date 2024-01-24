#include <iostream>
#include <vector>

// Define grid parameters
const int N = 10; 
const int size = (N + 2) * (N + 2);

// Define single-dimensional arrays for velocity and density
double u[size] = {0.0};
double v[size] = {0.0};
double u_prev[size] = {0.0};
double v_prev[size] = {0.0};
double dens[size] = {0.0};
double dens_prev[size] = {0.0};

// Macro for indexing
#define IX(i, j) ((i) + (N + 2) * (j))

// Function to set boundary conditions
void set_bnd(int N, int b, double* x) {
    // Implementation of setting boundary conditions goes here
    
    // as of yet to be figured out

}

// Function to update density matrix based on advection
void advect(int N, int b, double* d, const double* d0, const double* u, const double* v, double dt) {
    double dt0 = dt * N;

    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            double x = i - dt0 * u[IX(i, j)];
            double y = j - dt0 * v[IX(i, j)];

            // Ensure x and y are within bounds
            if (x < 0.5) x = 0.5;
            if (x > N + 0.5) x = N + 0.5;
            if (y < 0.5) y = 0.5;
            if (y > N + 0.5) y = N + 0.5;

            int i0 = static_cast<int>(x);
            int i1 = i0 + 1;
            int j0 = static_cast<int>(y);
            int j1 = j0 + 1;

            double s1 = x - i0;
            double s0 = 1 - s1;
            double t1 = y - j0;
            double t0 = 1 - t1;

            // Linear interpolation for advection
            d[IX(i, j)] = s0 * (t0 * d0[IX(i0, j0)] + t1 * d0[IX(i0, j1)]) +
                          s1 * (t0 * d0[IX(i1, j0)] + t1 * d0[IX(i1, j1)]);
        }
    }

    // Set boundary conditions
    set_bnd(N, b, d);
}


// Function to add a source of density at specific coordinates
void addSource(double* x, int targetX, int targetY, double sourceValue) {
    int index = IX(targetX, targetY);
    x[index] += sourceValue;
}

// Function for diffusion
void diffuse(int N, int b, double* x, const double* x0, double diff, double dt) {
    double a = dt * diff * N * N;

    for (int k = 0; k < 5; k++) { // k is 20 in the original funciton
        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                int index = IX(i, j);
                x[index] = (x0[index] + a * (x[IX(i - 1, j)] + x[IX(i + 1, j)] +
                          x[IX(i, j - 1)] + x[IX(i, j + 1)])) / (1 + 4 * a);
            }
        }
        set_bnd(N, b, x);
    }
}

// Testing software
int main() {

    // Make initial changes to velocities and densities
    for (int i = 0; i < size; ++i) {
        u[i] = 1.0;
        v[i] = 2.0;
        dens_prev[i] = 0.0;
    }

    // Call the function to add a source of density at specific coordinates
    int sourceX = 3;  // Adjust the source X-coordinate 
    int sourceY = 7;  // Adjust the source Y-coordinate 
    double sourceValue = 1;  // Adjust the source value 
    addSource(dens_prev, sourceX, sourceY, sourceValue);

    // Output the result after adding source
    std::cout << "Result after adding Source:" << std::endl;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            int index = IX(i, j);
            std::cout << dens_prev[index] << ' ';
        }
        std::cout << std::endl;
    }    

    // Call the diffusion function
    double diff = 0.1;  // Adjust the diffusion parameter as needed
    double dt = 0.1;    // Adjust the time step as needed
    diffuse(N, 0, dens, dens_prev, diff, dt);

    // Output the result after diffusion
    std::cout << "Result after diffusion:" << std::endl;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            int index = IX(i, j);
            std::cout << dens[index] << ' ';
        }
        std::cout << std::endl;
    }

    advect(N, 0, dens, dens_prev, u, v, dt);

    // Output the result after velocity addition
    std::cout << "Result after velocity fields:" << std::endl;
    for (int i = 1; i <= N; ++i) {
        for (int j = 1; j <= N; ++j) {
            int index = IX(i, j);
            std::cout << dens[index] << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}
