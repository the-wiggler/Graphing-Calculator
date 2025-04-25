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
        std::vector<double> x_arr;
        std::vector<double> y_arr;

        void executeFunctionCalculation() {
            // sets the range that the function should exist in
            function_res = 20;
            double domain_min = -5;
            double domain_max = 5;

            //how often x should should graph points as based on the path resolution
            double function_res_render_increment = (domain_max - domain_min) / function_res;
            std::cout << "Function increments: " << function_res_render_increment << std::endl;

            //starts x at the minimum bounds to graph function across bounds
            double x = domain_min;
            // Loop that creates all of the points in the graph
            for (int i = 0; i < function_res + 1; i++) {
                // the function
                double y = 0.5 * x;

                // appends the x and y coordinates to their own arrays
                x_arr.push_back(x);
                y_arr.push_back(y);

                std::cout << y_arr[i] << std::endl;

                x += function_res_render_increment;
             }
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
        for (int i = 0; i < outputs.function_res + 1; i++) {
            SDL_RenderDrawPoint(renderer, outputs.x_arr[i], outputs.y_arr[i]);
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
            }
        }

    }


return 0;
}