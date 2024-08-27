My purpose here is to learn something about making an application using SDL and Dear ImGui, as I am used to using Qt and wanted to learn a different way. I've implemented an interface to play with the parameters of the model, as well as to choose cool colormaps to make it look nice. You can choose some presets of these parameters as well as a couple of samples of some more complex oscillators. Where does it go next? I'd like to be able to load presets from files downloaded from https://conwaylife.com/wiki/, and to allow you to copy and paste RLE encoded patterns from the same site.
If I am still having fun I might play around with different SDL backends and/or implement and optimize the rules with CUDA to make it run as fast as I can for some reason (though it runs plenty fine as it is..). 

https://github.com/Cervidellus/GameOfLife/assets/17419420/796a69a1-8693-4318-b78d-75960b464bd2

It is already a fun toy to play with. Here is what you can do:

1. Change the speed of model updates.
  Sometimes it is fun to slow it down so you can see what is going on.
  At the top of the Options menu you can select a desired FPS.
2. Change the rules of the game of life.
   In the Game of Life Parameters window you can change the size of the model as well as how the rules are implemented.
   This can yield some strange and fun results. I
   To change the size, you first have to pause the model and then click generate.
3. Change the colors.
   Choose the color map under the visualization tab.
   The default right now is DualColor, which allows you to choose different colors for alive vs. dead cells.
   Try selecting something like Plasma instead!
4. Select a preset.
   In the presets menu select a preset.
   I've put several well known conway's game of life models in there.
   There aren't many yet, but I plan on adding more when I get the time.
5. Load a model from file, or copy and paste from the web!
   To load from file rowse the excellent https://conwaylife.com/wiki/ and download it.
   Then under the presets menu select From File and point to it's path.
   Or, you can just copy the RLE encoded model from the same website, select From String and paste it in there!
6. Get some debug info under the Timer Results tab. 
   
  





