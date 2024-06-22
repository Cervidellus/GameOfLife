//I need an abstract model class that will act as an interface for rendering.
//I can have different implementations underneath

//I'll have a simple CPU square grid representation.Use a vector of vectors for rows and columns
// Maybe I should do a sparse representation? I can build my own data type or use Eigen. 
//I'll implement a square grid with CUDA
//I'll implement a grid with intel's sycl or dpc++. 
//AdaptiveCpp(hipSYCL) from AMD
//Vulkan?

#include "modelparameters.hpp"

class SDL_Texture;

class AbstractModel {
public:
	AbstractModel() = default;
	~AbstractModel() = default;

	//I think eventually the model parameters will not be in abstract class. I think I need it here now just to get it going.
	ModelParameters modelParameters;

	virtual void initialize() = 0;
	virtual void update() = 0;	
	//Some interface for rendering. Should it be an SDL? Or should I give it the window texture?


	//I could also have a drawing strategy pattern.. so if I want to do squares, or circles, or have things run into each other. 
	//I don't think the different drawing strategies belong in the abstract class though.
	virtual void draw(SDL_Texture* texture) = 0;
	//SHould I also add a draw that specifies a viewport? That way I only have to draw what is visible.

	//I could have something that provides an IMGUI interface for options, and connects callbacks to set parameters

	//I'll add that after I get it working though. 

	//I could also have an event handler

	//

private:

	//I need an underlying storage type... should I have a vector of vectors or just a vector, and length and width params? 

	
};
