#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include <vector>

#define WINDOW_SIZE_X 1000
#define WINDOW_SIZE_Y 800

class numericalOutputs {
    private:

    public:
        int function_res;
        std::vector<double> x_arr, y_arr;
        double y_min, y_max, x_min, x_max, x_range, y_range;

        void executeFunctionCalculation() {
            // sets the range that the function should exist in
            function_res = 1000;
            double domain_min = -50;
            double domain_max = 5;

            //how often x should should graph points as based on the path resolution
            double function_res_render_increment = (domain_max - domain_min) / function_res;

            //starts x at the minimum bounds to graph function across bounds
            double x = domain_min;
            // Loop that creates all of the points in the graph
            for (int i = 0; i < function_res + 1; i++) {
                // the function
                double y = sin(10 * x) * cos(12 * pow(x, 2)) * exp(-0.1 * pow(x, 2)) +
                    0.4 * tanh(5 * sin(8 * x)) * cos(20 * x) +
                    0.2 * sin(30 * x) * pow(cos(x * x), 2) * sin(x * x * x) +
                    0.15 * sin(50 * x) / (1 + pow(x, 2)) * cos(x * x * 5) *
                    (sin(x * 25) > 0 ? sin(x * 25) : 0.3 * cos(x * 15));

                x_arr.push_back(x);
                y_arr.push_back(y);

                x += function_res_render_increment;
             }

            // finds max and min values
            x_min = domain_min;
            x_max = domain_max;
            y_min = y_arr[0];
            y_max = y_arr[0];
            for (double y : y_arr) {
                if (y < y_min) y_min = y;
                if (y > y_max) y_max = y;
            }
            
            // range values
            x_range = domain_max - domain_min;
            y_range = y_max - y_min;
        }
};



int SDL_main(int argc, char* argv[]) {
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Graphing Calculator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_SIZE_X, WINDOW_SIZE_Y, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // LOOP FOR THE WINDOW PROGRAM                                                                           //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    while (running) {
        numericalOutputs outputs;

        outputs.executeFunctionCalculation();

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

        int prev_x_point;
        int prev_y_point;
        for (int i = 0; i <= outputs.function_res; i++) {
            int scaled_x_point = static_cast<int>((outputs.x_arr[i] - outputs.x_min) * WINDOW_SIZE_X / outputs.x_range);
            int scaled_y_point = static_cast<int>(WINDOW_SIZE_Y - (outputs.y_arr[i] - outputs.y_min) * WINDOW_SIZE_Y / outputs.y_range);

            if (i > 0) { SDL_RenderDrawLine(renderer, prev_x_point, prev_y_point, scaled_x_point, scaled_y_point); }

            prev_x_point = scaled_x_point;
            prev_y_point = scaled_y_point;
        }

        SDL_RenderPresent(renderer);
        


        // stops the rendering loop until a certain command is executed
        bool wait_for_reset = true;
        while (wait_for_reset) {
            while (SDL_PollEvent(&event)) {
                // quits the program
                if (event.type == SDL_QUIT) {
                    running = false;
                    wait_for_reset = false;
                // re-renders the function when the letter "r" is pressed
                } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                    wait_for_reset = false;
                }
                SDL_Delay(16);
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

    }


return 0;
}