#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//const char* ttf_filename = "./sample.ttf";
const char* ttf_filename = "./DroidSans.ttf";

SDL_Texture* loadTexture(char* filename, SDL_Renderer* ren) {
	printf("[%s, %d] IMG_LoadTexture, filename = %s!\n", __func__, __LINE__, filename);
	SDL_Texture *texture = IMG_LoadTexture(ren, filename);
	if(texture == nullptr) {
		printf("[%s, %d] IMG_LoadTexture failed!\n", __func__, __LINE__);
	}
	return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
	printf("[%s, %d] x = %d, y = %d, width = %d, height = %d\n", __func__, __LINE__, x, y, w, h);
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_RenderCopy(ren, tex, NULL, &rect);
}

void renderTextureOld(SDL_Texture *tex, SDL_Renderer *ren, int x, int y) {
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	printf("[%s, %d] width = %d, height = %d\n", __func__, __LINE__, w, h);
	renderTexture(tex, ren, x, y, w, h);
}

SDL_Texture* renderText(char* message, SDL_Color color, TTF_Font *font, SDL_Renderer* ren) {
	SDL_Surface *surface = TTF_RenderText_Blended(font, message, color);
	if(surface == nullptr) {
		printf("[%s, %d] TTF_RenderText_Blended failed!\n", __func__, __LINE__);
		return nullptr;
	}	

	SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surface);
	if(tex == nullptr) {
		printf("[%s, %d] SDL_CreateTextureFromSurface failed!\n", __func__, __LINE__);
		return nullptr;
	}

	SDL_FreeSurface(surface);
	return tex;
}

int main(int argc, char **argv){
	int user_quit = 0;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		printf("[%s, %d] SDL_Init failed!\n", __func__, __LINE__);
		return 1;
	}

	// initialize SDL_image
	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		printf("[%s, %d] IMG_Init failed!\n", __func__, __LINE__);
		return 1;
	}

	// initialize SDL_TTF
	if(TTF_Init() != 0) {
		printf("[%s, %d] TTF_Init failed!\n", __func__, __LINE__);
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Test2", 100, 100, 1280, 720, SDL_WINDOW_SHOWN);

	if(win == nullptr) {
		printf("[%s, %d]  SDL_CreateWindow failed!\n", __func__, __LINE__);
		return 1;
	}

	//SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
	if(ren == nullptr) {
		printf("[%s, %d]  SDL_CreateRenderer failed!\n", __func__, __LINE__);
		return 1;
	}

	SDL_Texture *background = loadTexture("./pnggrad16rgba.png", ren);
	//SDL_Texture *background = loadTexture("./blue.bmp", ren);
	SDL_Texture *image = loadTexture("./000070-blue-white-pearl-icon-media-a-media22-arrow-forward1.png", ren);
	if(background == nullptr || image == nullptr) {
		printf("[%s, %d]  loadTexture failed!\n", __func__, __LINE__);
		return 1;
	}

	SDL_RenderClear(ren);

	//renderTextureOld(background, ren, 0, 0);  // draw background image.
	renderTexture(background, ren, 0, 0, 1280, 720);  // draw background image.

	int tmpW, tmpH;
	SDL_QueryTexture(image, NULL, NULL, &tmpW, &tmpH);
	int rW = tmpW / 5;  // original image is too big, let's scale down 5x.
	int rH = tmpH / 5;
	renderTexture(image, ren, 10, 10, rW, rH);

	SDL_RenderPresent(ren);  // present the updated screen to show the result.

	//SDL_Delay(5000);
	// main loop
	SDL_Event evt;
	int fontSize = 80;
	TTF_Font *font = TTF_OpenFont(ttf_filename, fontSize);
	if(font == nullptr) {
		printf("[%s, %d]  TTF_OpenFont failed!\n", __func__, __LINE__);
		TTF_CloseFont(font);
		return 1;
	}

	while(!user_quit) {
		while(SDL_PollEvent(&evt)) {
			if(evt.type == SDL_KEYDOWN) {
				switch(evt.key.keysym.sym) {
					case SDLK_q:  // when receive 'q' key, leave the program.
						printf("[%s, %d] Got 'q' key press, let's leave the program !\n", __func__, __LINE__);
						user_quit = 1;
					break;
					case SDLK_a:  // draw text
						SDL_Color color = {128, 128, 128};
						SDL_Texture *imageText = renderText("Test Installation!!!", color, font, ren); 
						if(imageText == nullptr) {
							printf("[%s, %d] Got 'a' key press, renderText failed !\n", __func__, __LINE__);
							return 1;
						}
						//SDL_RenderClear(ren);
						renderTextureOld(imageText, ren, 50, 300);  // draw text image.
						SDL_RenderPresent(ren);  // present the updated screen to show the result.
					break;
				}
			}
		}

	}

	TTF_CloseFont(font);
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	IMG_Quit();
	SDL_Quit();

	return 0;
}
