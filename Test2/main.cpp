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

// timer callback
Uint32 draw_progress_func(Uint32 interval, void *param) {
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = 0;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);
	return interval;
}

void displayUsage(SDL_Renderer *ren) {

	int fontSize = 40;
	TTF_Font *font = TTF_OpenFont(ttf_filename, fontSize);
	if(font == nullptr) {
		printf("[%s, %d]  TTF_OpenFont failed!\n", __func__, __LINE__);
		TTF_CloseFont(font);
		return;
	}

	SDL_Color color = {0x28, 0xFF, 0x28, 0x80};
	SDL_Texture *imageText = renderText("[Usage]", color, font, ren); 
	renderTextureOld(imageText, ren, 50, 520);  // draw text image.
	SDL_DestroyTexture(imageText);

	imageText = renderText("a : show text", color, font, ren); 
	renderTextureOld(imageText, ren, 50, 560);  // draw text image.
	SDL_DestroyTexture(imageText);

	imageText = renderText("b : show preogrss", color, font, ren); 
	renderTextureOld(imageText, ren, 50, 600);  // draw text image.
	SDL_DestroyTexture(imageText);

	imageText = renderText("q : exit", color, font, ren); 
	renderTextureOld(imageText, ren, 50, 640);  // draw text image.
	SDL_DestroyTexture(imageText);

	SDL_RenderPresent(ren);  // present the updated screen to show the result.
}

int main(int argc, char **argv){
	static int SCREEN_WIDTH = 1280;
	static int SCREEN_HEIGHT = 720;
	int user_quit = 0;
	SDL_TimerID timer;  // used to draw the progress bar.
	int icon_start_x = 0;
	int icon_gap = 30;

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

	SDL_Window *win = SDL_CreateWindow("Test2", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
	renderTexture(background, ren, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);  // draw background image.

	int tmpW, tmpH;
	SDL_QueryTexture(image, NULL, NULL, &tmpW, &tmpH);
	int rW = tmpW / 5;  // original image is too big, let's scale down 5x.
	int rH = tmpH / 5;
	renderTexture(image, ren, 10, 10, rW, rH);
	icon_start_x += 10;

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

	displayUsage(ren);

	while(!user_quit) {
		while(SDL_PollEvent(&evt)) {
			if(evt.type == SDL_KEYDOWN) {
				switch(evt.key.keysym.sym) {
					case SDLK_q:  // when receive 'q' key, leave the program.
						printf("[%s, %d] Got 'q' key press, let's leave the program !\n", __func__, __LINE__);
						user_quit = 1;
					break;
					case SDLK_a:  // draw text
					{
						SDL_Color color = {0x4D, 0xFF, 0xFF, 0xFF};
						SDL_Texture *imageText = renderText("Test Installation!!!", color, font, ren); 
						if(imageText == nullptr) {
							printf("[%s, %d] Got 'a' key press, renderText failed !\n", __func__, __LINE__);
							return 1;
						}
						//SDL_RenderClear(ren);
						renderTextureOld(imageText, ren, 50, 300);  // draw text image.
						SDL_RenderPresent(ren);  // present the updated screen to show the result.
					}
					break;
					case SDLK_b:
					{
						int interval = 5;  // 5ms
						timer = SDL_AddTimer(interval, draw_progress_func, NULL);
						printf("[%s, %d] timer = %d!\n", __func__, __LINE__, timer);
					}
					break;
				}
			} else if(evt.type == SDL_USEREVENT) {  // trigger by timer
				printf("[%s, %d] Got SDL_USEREVENT !\n", __func__, __LINE__);
				int tmpW, tmpH;
				SDL_QueryTexture(image, NULL, NULL, &tmpW, &tmpH);
				int rW = tmpW / 5;  // original image is too big, let's scale down 5x.
				int rH = tmpH / 5;

				icon_start_x += icon_gap;
				if(icon_start_x > SCREEN_WIDTH) {
					SDL_RemoveTimer(timer);
					
					SDL_Color color = {0xFF, 0x00, 0x00, 0xFF};
					SDL_Texture *imageText = renderText("Installation is done!!!", color, font, ren); 
					renderTextureOld(imageText, ren, 50, 200);  // draw text image.
					SDL_RenderPresent(ren);  // present the updated screen to show the result.
					break;
				}
				renderTexture(image, ren, icon_start_x, 10, rW, rH);
				SDL_RenderPresent(ren);  // present the updated screen to show the result.
			}
		}

	}

	SDL_Delay(5000);

	SDL_RemoveTimer(timer);
	TTF_CloseFont(font);
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);

	IMG_Quit();
	SDL_Quit();

	return 0;
}
