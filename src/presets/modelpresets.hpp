#ifndef GAMEOFLIFE_MODELPRESETS_H
#define GAMEOFLIFE_MODELPRESETS_H

#include <model\modelparameters.hpp>

#include <string_view>

//Preset parameters for models.
//In order to add a new model, add a new ModelPresetName, a new getModelPresetStringView, and a new getModelPresetParameters
//If this gets to unwieldly, we can have the parmeters in a separate file.
//Buttons for the presets will be generated in the GUI using the ModelPresetName enum and the getModelPresetStringView function.

namespace ModelPresets {

	const ModelParameters randomParams = ModelParameters();

	const ModelParameters swissCheeseParams = ModelParameters(
		true,
		-1,
		-1,
		0.9f,
		5,
		8,
		1
	);

	const ModelParameters decompositionParams = ModelParameters(
		true,
		-1,
		-1,
		0.9f,
		5,
		8,
		3
	);

	const ModelParameters blinkerParams = ModelParameters(
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		3,
		3,
		"3o!"
	);

	const ModelParameters lightweightSpaceshipParams = ModelParameters(
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		40,
		40,
		"bo2bo$o4b$o3bo$4o!"
	);

	const ModelParameters blockerParams = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		110,
		110,
		"6bobob$5bo4b$2o2bo4bo$2obo2bob2o$4b2o!"
	};

	const ModelParameters nihoniumParams = {
	false,
	-1,
	-1,
	-1,
	2,
	3,
	3,
	58,
	37,
	"42b2o$42b2o5b2o$49b2o$13b2o$14bo$13bo33b2o$13b2o32b2o$53b2o$2o51b2o$b"
	"o$bob2o$2bo$14b2o$4bobo8bo$5bo7b3o$6bo46bo$3b4o45bobo$2bo49bobo$2b2ob"
	"2o44b2ob2o$3bobo49bo$3bobo45b4o$4bo46bo$42b3o7bo$42bo8bobo$42b2o$55bo"
	"$53b2obo$56bo$3b2o51b2o$3b2o$9b2o32b2o$9b2o33bo$43bo$43b2o$7b2o$7b2o5b"
	"2o$14b2o!"
	};

const ModelParameters gabrielsPOneThirtyEightParams = {
	false,
	-1,
	-1,
	-1,
	2,
	3,
	3,
	33,
	33,
	"8b3o10b$7bo2bo10b$7bo2bo10b$7b2o12b4$17b3ob$17bo2bo$20bo$3o15b3o$o20b$"
	"o2bo17b$b3o17b4$12b2o7b$10bo2bo7b$10bo2bo7b$10b3o!"
	};

	const ModelParameters backrakeOnePufferVarTwo = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		30,
		800,
		"5b3o11b3o$4bo3bo9bo3bo$3b2o4bo7bo4b2o$2bobob2ob2o5b2ob2obobo$b2obo4bo"
		"b2ob2obo4bob2o$o4bo3bo2bobo2bo3bo4bo$12bobo$2o7b2obobob2o7b2o$12bobo$"
		"6b3o9b3o$6bo3bo5bo3bo$6bobo9bobo!"
	};

	const ModelParameters connectedPhoenix = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		58,
		95,
		"3bo$3bobo$bo5bo$7bobo9bo7bo$2o15bobo5bobo$10b2o9bobo5bo$2bo12b2o6bo$10b"
		"o18b2o$2b2o12bo$8bobo17bo$4bobo10b2o$6bo3bobo14b2o$14bobo2bobo$10bo3b"
		"o6bo7bo$8bobo12bo$29b2o$6b2o3b2o10b2o$28bo$6bo5bo9bo$4bo15bobo4b2o$2b"
		"obo8b2o$18b2o3b2o4bo$2o11bo$18bo4bo5b2o$bo9b2o$3bo12b2o3b2o5bo$3bobo6b"
		"o$7bo9bo3bo5b2o$7bobo2bobo4bo$19bo6bo$10b2o2bob2o$25b2o$12bobobo$24bo"
		"$14bo$23b2o$15b2o$22bo$16bo$21b2o$17b2o$23bo$18bo$20bobob2o$20bo$26b2o"
		"2$28b2o2$30b2o$37bo$32b2obobo$39bo$34bo$39b2o$35b2o$41bo$35bo$41b2o$33b"
		"2o$43bo$33bo$41bobobo$31b2o$40b2obo2b2o$31bo6bo$38bo4bobo2bobo$29b2o5b"
		"o3bo9bo$45bo6bobo$29bo5b2o3b2o12bo$45b2o9bo$27b2o5bo4bo$44bo11b2o$28b"
		"o4b2o3b2o$43b2o8bobo$29b2o4bobo15bo$35bo9bo5bo$29bo$33b2o10b2o3b2o$27b"
		"2o$34bo12bobo$28bo7bo6bo3bo$36bobo2bobo$29b2o14bobo3bo$39b2o10bobo$29b"
		"o17bobo$41bo12b2o$27b2o18bo$34bo6b2o12bo$28bo5bobo9b2o$30bobo5bobo15b"
		"2o$30bo7bo9bobo$50bo5bo$52bobo$54bo!"
	};

	const ModelParameters doubleX = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		131,
		63,
		"b2o125b2ob$b2o125b2ob$7b3o111b3o7b$6bo3bo109bo3bo6b$6b2ob2o41b2o23b2o"
		"41b2ob2o6b$30b2o19b4o21b4o19b2o30b$6b2ob2o17b2ob2o18b2ob2o19b2ob2o18b"
		"2ob2o17b2ob2o6b$8bo19b4o21b2o21b2o21b4o19bo8b$16b2o11b2o69b2o11b2o16b$"
		"16bobo93bobo16b$16bo97bo16b$bo7bo111bo7bob$o2bo3bo2bo45bobo13bobo45bo"
		"2bo3bo2bo$4bobo49b2o15b2o49bobo4b$4bobo50bo15bo50bobo4b$4bobo117bobo4b"
		"$o2bo3bo2bo109bo2bo3bo2bo$b3o3b3o111b3o3b3ob3$27b3o71b3o27b$27bo75bo"
		"27b$28bo73bo28b$46bo37bo46b$44b2o39b2o44b$45b2o37b2o45b2$b2o125b2ob$b"
		"2o125b2ob3$39b2o49b2o39b$39bobo47bobo39b$39bo51bo39b$b2o125b2ob$b2o30b"
		"obo59bobo30b2ob$7b3o23b2o61b2o23b3o7b$6bo3bo23bo61bo23bo3bo6b$6b2ob2o"
		"109b2ob2o6b2$6b2ob2o109b2ob2o6b$8bo113bo8b2$50b3o25b3o50b$50bo29bo50b$"
		"bo7bo41bo27bo41bo7bob$o2bo3bo2bo12bo83bo12bo2bo3bo2bo$4bobo14b2o85b2o"
		"14bobo4b$4bobo15b2o83b2o15bobo4b$4bobo117bobo4b$o2bo3bo2bo109bo2bo3bo"
		"2bo$b3o3b3o111b3o3b3ob3$16bo2bo42b2o3b2o42bo2bo16b$20bo19b4o18bobobobo"
		"18b4o19bo20b$16bo3bo18bo3bo18bo5bo18bo3bo18bo3bo16b$17b4o22bo43bo22b4o"
		"17b$39bo2bo45bo2bo39b3$b2o125b2ob$b2o125b2o!"
	};

	const ModelParameters frothingPufferRake = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		249,
		800,
		"72bo85bo$71b3o83b3o$52boboo14bo3bo14boobo45boboo14bo3bo14boobo$51boob"
		"ooboobo10b3o10boboobooboo43boobooboobo10b3o10boboobooboo$50bobboboboob"
		"oo8b5o8booboobobobbo41bobbobobooboo8b5o8booboobobobbo$51bo6bo5bo4boo3b"
		"oo4bo5bo6bo43bo6bo5bo4boo3boo4bo5bo6bo$59bo3b4obo7bob4o3bo59bo3b4obo7b"
		"ob4o3bo$63bo3b3obobob3o3bo67bo3b3obobob3o3bo$58boobobbo3boo5boo3bobbob"
		"oo57boobobbo3boo5boo3bobboboo$57bo4bobbobobbooboobbobobbo4bo55bo4bobbo"
		"bobbooboobbobobbo4bo$56boobobo6boo5boo6boboboo53boobobo6boo5boo6bobob"
		"oo$54boobobo6booboboboboboo6boboboo49boobobo6booboboboboboo6boboboo$"
		"52boobo9bo3bobbobbo3bo9boboo45boobo9bo3bobbobbo3bo9boboo$51bo3bo9bobob"
		"oo3boobobo9bo3bo43bo3bo9boboboo3boobobo9bo3bo$52bobo11bobobo3bobobo11b"
		"obo45bobo11bobobo3bobobo11bobo$69bobobobo79bobobobo$66bo11bo73bo11bo$"
		"64boobo3bobo3boboo69boobo3bobo3boboo$67bo4bo4bo75bo4bo4bo$67bobooboboo"
		"bo75bobooboboobo$66boobo5boboo73boobo5boboo$65bobobo5bobobo71bobobo5bo"
		"bobo$64boobobob3oboboboo69boobobob3oboboboo$67bobo5bobo75bobo5bobo$64b"
		"ooboboo3booboboo69booboboo3booboboo$43boboo24b3o24boobo27boboo24b3o24b"
		"oobo$39b3obooboboo18b9o18booboboob3o19b3obooboboo18b9o18booboboob3o$"
		"38bo6boobb4o15boo5boo15b4obboo6bo17bo6boobb4o15boo5boo15b4obboo6bo$39b"
		"oo3bo3bo4bo12boo9boo12bo4bo3bo3boo19boo3bo3bo4bo12boo9boo12bo4bo3bo3b"
		"oo$51b3o5boo9bobobo9boo5b3o43b3o5boo9bobobo9boo5b3o$57boboo5bobboo3boo"
		"bbo5boobo55boboo5bobboo3boobbo5boobo$54boboo3boo19boo3boobo49boboo3boo"
		"19boo3boobo$53booboobobobo4bob5obo4bobobobooboo47booboobobobo4bob5obo"
		"4bobobobooboo$53boobo4bob3ob11ob3obo4boboo47boobo4bob3ob11ob3obo4boboo"
		"$57boobo3bob13obo3boboo55boobo3bob13obo3boboo$64bo15bo69bo15bo$65bo13b"
		"o71bo13bo$$68b9o77b9o$$69boo3boo13bo51bo13boo3boo$72bo12b3oboo49boob3o"
		"12bo$67bo9bo6bo5b3o45b3o5bo6bo9bo$67bo9bo7boo7boo39boo7boo7bo9bo$66bo"
		"3bobobo3bo10bo4boo39boo4bo10bo3bobobo3bo$66bo4bobo4bo9bo4b3o39b3o4bo9b"
		"o4bobo4bo$65bo4bo3bo4bo7boo3boo5b3o5bo15bo5b3o5boo3boo7bo4bo3bo4bo$65b"
		"5o5b5o5boobboobb3obbo6b3ob3o7b3ob3o6bobb3obboobboo5b5o5b5o$85b3oboo6b"
		"oo5boo6bo5bo6boo5boo6boob3o$86booboobboo3b4obobbo3boo7boo3bobbob4o3boo"
		"bbooboo$$64b3o11b3o20bo27bo20b3o11b3o$69bo5bo25bobo23bobo25bo5bo$68boo"
		"5boo77boo5boo$66bo3bobobo3bo73bo3bobobo3bo$66bobbo5bobbo73bobbo5bobbo$"
		"66bo3bobobo3bo73bo3bobobo3bo$67bobo5bobo75bobo5bobo$68boo5boo77boo5boo"
		"3$69boo3boo79boo3boo$69boo3boo79boo3boo$$78bo73bo$78bobo69bobo$78boo"
		"71boo$$69boo3boo79boo3boo$69boo3boo79boo3boo$$69boo3boo79boo3boo$69boo"
		"3boo79boo3boo4$75boo77boo$74boboo75boobo$73boobb3o71b3obboo$73b3ob3o"
		"71b3ob3o$73bobboboo71boobobbo$73bobbobo73bobobbo$73boobobo73boboboo$"
		"71bobbobobo73bobobobbo$70bobo5boo71boo5bobo$$70b3o8bo67bo8b3o$81bo67bo"
		"$81bo67bo$$77b3o71b3o5$45boob3o129b3oboo$45boo4boo125boo4boo$44bobbobo"
		"3bo20bobo77bobo20bo3bobobbo$51b3obb3o3bobo9bobo77bobo9bobo3b3obb3o$53b"
		"3obobobbooboo8bobbo73bobbo8booboobbobob3o$53bo4bo8bobo6b3o73b3o6bobo8b"
		"o4bo$54boo7boboo3boo87boo3boobo7boo$64b3obo8boo73boo8bob3o$66boboo7b3o"
		"71b3o7boobo$66bobo10bo71bo10bobo$67bo11boo69boo11bo$79bo71bo$70bo10bo"
		"21booboo15booboo21bo10bo$70bo9bobo18boobobooboo9boobooboboo18bobo9bo$"
		"69boo4boo3boo16b3obobb3obb4o3b4obb3obbob3o16boo3boo4boo$68boo5boo3bo"
		"13b3o4b3o5bo4bobo4bo5b3o4b3o13bo3boo5boo$68bobobboboo16bobobob3obo8boo"
		"3boo8bob3obobobo16boobobbobo$72bobbo17bobobob4o25b4obobobo17bobbo$72b"
		"3o19bobo4bobbo21bobbo4bobo19b3o$74bo28bo23bo28bo$73bo21bo39bo21bo$70bo"
		"bbo21bo39bo21bobbo7$104b3oboo11boob3o$102boo4boo11boo4boo$101bo3bobobb"
		"o9bobbobo3bo$96b3obb3o23b3obb3o$95bobob3o27b3obobo$86boo8bo4bo7boo9boo"
		"7bo4bo8boo$86booboo8boo5booboo9booboo5boo8booboo$80boo4boobb4o9b4obbob"
		"o7bobobb4o9b4obboo4boo$80booboo4bo4bo7bo4bo4bo5bo4bo4bo7bo4bo4booboo$"
		"79bo3boo7boo9boo7bo5bo7boo9boo7boo3bo$83boo27bo5bo27boo$108b3o9b3o$"
		"107bo15bo$108boboo7boobo$112bo5bo$108boo11boo$107bo3bo7bo3bo$110bo9bo$"
		"108bo13bo$108bo13bo$114b3o$108boo4b3o4boo$113bo3bo$107b3o11b3o$66boo"
		"41bobbo5bobbo41boo$66boo40bo3boo3boo3bo40boo$114bobo$107b3o5bo5b3o$"
		"107b4obooboboob4o$109bobb7obbo$110b4o3b4o$109b3o7b3o$108bo4bo3bo4bo$"
		"107bo3bo3bo3bo3bo$65bo46bob3obo46bo$65boo40bob4o5b4obo40boo$64bo42bo4b"
		"3ob3o4bo42bo$65b3o42boo3bo3boo42b3o$66bo46bo3bo46bo$48bo60boobbooboobb"
		"oo60bo$44b3ob3o57bobobbooboobbobo57b3ob3o$43bo6boo56bo6bo6bo56boo6bo$"
		"44boo3bobbo3bo50boo6bo6boo50bo3bobbo3boo$54b4o49bob3obbobobb3obo49b4o$"
		"54bo3bo52bo7bo52bo3bo$52bobbobo52bobo5bobo52bobobbo$53bo53b3obbob3obo"
		"bb3o53bo$41boob3o59boboboboo3boobobobo59b3oboo$41boo4boo32b3o22bobobob"
		"oo3boobobobo22b3o32boo4boo$40bobbobo3bo33bo23boob3obobob3oboo23bo33bo"
		"3bobobbo$47b3obb3o27bo25boo3booboo3boo25bo27b3obb3o$49b3obobo54b3obobo"
		"b3o54bobob3o$49bo4bo52bo4bobobobo4bo52bo4bo$50boo60bobobobo60boo$66boo"
		"46bobo46boo$66boo36boo8bobo8boo36boo$103bobo7booboo7bobo$105bo19bo$"
		"113bo3bo$112bo5bo$51boob3o20b3oboo30bo3bo30boob3o20b3oboo$51boo4boo16b"
		"oo4boo65boo4boo16boo4boo$50bobbobo3bo14bo3bobobbo63bobbobo3bo14bo3bobo"
		"bbo$57b3obb3o4b3obb3o77b3obb3o4b3obb3o$59b3obobobbobob3o81b3obobobbobo"
		"b3o$59bo4bo4bo4bo12bo55bo12bo4bo4bo4bo$60boo10boo9b3ob3o51b3ob3o9boo"
		"10boo12bo$82bo6boo49boo6bo33bobo$83boo3bobbo3bo39bo3bobbo3boo18bo14bo"
		"3bo14bo$93b4o37b4o24b3ob3o13b3o13b3ob3o$93bo3bo35bo3bo23bo6bob3o21b3ob"
		"o6bo$46b3o42bobbobo37bobobbo22boo3bo5bo6boo3boo6bo5bo3boo$46b3o43bo45b"
		"o31b7o3boo3boo3b7o$26boob3o13bo3bo13b3oboo99bobboo3boobb3ob3obboo3boo"
		"bbo$26boo4booboo21booboo4boo100boobo4boo9boo4boboo$25bobbobo4boo7bo5bo"
		"7boo4bobobbo105b4obbooboobb4o$32boobobboo4booboboo4boobboboo104boob4o"
		"7booboo7b4oboo$38boboo11boobo109boboboo5boobob3oboboo5boobobo$33b3oboo"
		"bbo3bo3bo3bobboob3o101boobo10boobo5boboo10boboo$33bo3b3obo4bobo4bob3o"
		"3bo101boobo9bobobo5bobobo9boboo$31booboboo4bobbo3bobbo4booboboo99bobo"
		"10bo3bo5bo3bo10bobo$30bobobbo8bobobobo8bobbobo117bobo$28b3obobbo5boobo"
		"bbobboboo5bobbob3o109bo3b3ob3o3bo$27boobo9bobobob3obobobo9boboo108bob"
		"5ob5obo$26bo3bo9bobobbooboobbobo9bo3bo107bobo9bobo$41b3obbobobb3o124bo"
		"booboboobo$42boboo3boobo124boobob3oboboo$40boo3bo3bo3boo123bobob3obobo"
		"$40bobo4bo4bobo120booboboo3booboboo$42bobbobobobbo122boobobo5boboboo$"
		"42bob3ob3obo123bobobo5bobobo$42bobo5bobo104bo21boo5boo21bo$39boobobobb"
		"obboboboo94boob3ob3o20b7o20b3ob3oboo$39boobobobbobboboboo94boo4bobboob"
		"oo19bo19booboobbo4boo$42bobob3obobo96bobboboo3bobboo14bobobbobbobo14b"
		"oobbo3boobobbo$41bobb7obbo107bobbo5bo6boobo5boboo6bo5bobbo$15boboob3o"
		"49b3oboobo85bobb4o5boo3bobo3boo5b4obbo$14booboo4boobo16bo7bo16boboo4b"
		"ooboo87bo3bo6boo5boo6bo3bo$13bobbobobbo3b3o18b3o18b3o3bobbobobbo81b3o"
		"bboboboo5b9o5boobobobb3o$14bo5bo3bo3bo13b3o5b3o13bo3bo3bo5bo85bobboobo"
		"boo15booboboobbo$27boo4b3o5boob3ob3oboo5b3o4boo100boobo3boboo9boobo3bo"
		"boo$27boobboobo9b3ob3o9boboobboo107bobob9obobo13b3o$28boo4boboo9bo9boo"
		"bo4boo109b15o13bo$33bobbobo3boo7boo3bobobbo116b11o14boo$28boobobbobobo"
		"bo13bobobobobboboo130boboobboobb3o$24boo6bo8bo11bo8bo133boobooboo5bo4b"
		"oo$23boobo12bob13obo122boo3bo3boo6bobboo3boboo4booboo$23boobo20bo129bo"
		"booboboboobo11b4o6boo3bo$21b3oboo3bo16bo128boo11boo6boboboo8boo$16bo3b"
		"o4bobb3ob3o10booboo127bobo7bobo9bo$15b4o4b3obboo5bo8bobobobo127boo7boo"
		"7boo18bo$14bo3bo9boobobbo7bobooboboobo59bo5bo60bobo3bobo7boobbo4boobo"
		"3bo3boob3o$15bobobbo4boo4bo12bobobobo61bo5bo58boo9boo7b3oboobobooboob"
		"oobob3obbo3b3o$19bo9bob3o8bo9bo59bo5bo58boo9boo3bobbo5boboo6bo9bobbobo"
		"boo$14bo16bo3bo5boo9boo123bobooboboboobo4bob4obo3b3o3bobobbo3bobobbobo"
		"3bo$14b3obbo6bo3bobobbo3bo15bo124bo5bo12b3obboo10bo4boobbobbobo$3boo4b"
		"5o4bobob3ob3o6boo3bobo9bobo126bo3bo13boo4bo16b5o$ooboo3b3obbo4bobo3boo"
		"bbo3booboo5bo9bo129b3o15boboobo17boo$oo3bo4b5obobo3boo12bo3bobo9bobo$o"
		"bobbobbo3boo12bobboobobbo4boo11boo117bo28bobbo$3bob4o16boobboobbo137b"
		"4o26bobbo$25boboo142boboo$30bo11b3o5b3o$26b4o14bo5bo129boo$27boo17b3o"
		"10bo120boo$46b3o9bobo$47bo$177boo$176bobbo$49boo125bobo18b3ob3o$49boo"
		"126bo16b4obo4boobo$190bo3boo3boobo3b3o$189b4o5boobo3bo3bo$28booboo19b"
		"oo134bo3bobo3bo9bo$25boobooboboo16bobbo133bobobobo3bo$22b4obb3obbob3o"
		"14bobo122bo5boo4b3o4bobo$21bo4bo5b3o4b3o11bo122boo4bobbo4bo13b3ob3o$"
		"22boo8bob3obobobo133boo5b3o15b4obo4boobo$33b4obobobo154bo3boo3boobo3b"
		"3o$31bobbo4bobo117bo36b4o5boobo3bo3bo$21booboo6bo122b3ob3o33bo3bobo3bo"
		"9bo$18boobooboboo12bo4b3o106bo6boo14bo17bobobobo3bo$15b4obb3obbob3o9bo"
		"3bo3bo106boo3bobbo3bo7booboo7boo7b3o4bobo$14bo4bo5b3o4b3o8bo5bo115b4o"
		"6booboo6bobbo7bo$15boo8bob3obobobo8bo3bo21b3oboo89bo3bo5booboo6boob5o"
		"15b3oboo$26b4obobobo9b3o4b3o13boo4boo87bobbobo8bo8boobb4o13boo4boo$24b"
		"obbo4bobo16bo3bo11bo3bobobbo87bo24b3obboo10bo3bobobbo$25bo15bobo6bo5bo"
		"5b3obb3o118b3obobbo5b3obb3o$17boboo12bo15bo3bo3bo3bobob3o116boo3boo4bo"
		"4bobob3o$16boobooboo9bo10bo4bobbobobbo4bo4bo116b3obo4boo6bo4bo$15bobbo"
		"bobbo12boo11bo3bo3bo7boo117boboo16boo$16bo5boo4boo13boobo3bo5bo128boo"
		"bbo$25booboo3boobo3bobobo6bo3bo128bobb4o$25boo3bobbobboobobboobbo5b3o"
		"124boo4bo5bo$25boo6bobbobboo5boo131boo5b5o$34bobobbo6bo140b3o4$50boo$"
		"38boboo8boo$39b3o84bo26bo$40bo81b3ob3o11b4obo5b4o$121bo6bob3o7bobobboo"
		"3boo3bo$122boo3bo5bobboobobbo4bo5boboo$130boobobbobobboobobbo3b3obobo"
		"26b3o$76b3o6booboo13b3oboo23boobboboobbobbobbo4boboboboobo15b3o4boo$"
		"77boboo3bo3boboo6booboo4boo23boo3b3obbooboobobbobobobobooboo14bobboobb"
		"o$74boobo3bobo4bo4bo4boo4bobobbo25b3obbobbobo3bobobobobobo4bo13booboo"
		"bboo$73bobobb5o3b3o3b3obboboboo30bobbo5bo3bo4bobobobo5b3o14boboboo12b"
		"3oboo$71b3obobo3boobbobbo3bo4boo36bo17boboboboobb4o14b4o11boo4boo$67b"
		"3obobobobobbo4b3o3bobbo60bo5b3obbo15boo11bo3bobobbo$45b3oboboobo11bo6b"
		"obobob3o3bobo3bobbo3bo54bo7bo4bo23b3obb3o$28boboo13boo3bo3bo12booboobo"
		"bobobo3bobobobo4bo3bo48boo4bo3b4obboboo22bobob3o$27boobooboo10b4o3bobo"
		"9boo7bobobobo15boo37boboo8bobbooboobbobobobooboo23bo4bo$26bobbobobbo8b"
		"oob6o12boobboobbooboo56boobooboo4bo3bobboobo8bo28boo$27bo5boo4boo4bo"
		"20boobobboobo56bobbobobbo5b4o13boo$36booboo21bobb3oboboobb3o4bo50bo5b"
		"oo4bobbobobboo$36boo3bo21bobo9b3o3boboo8bo48boobobbobobboo$36boo29bobb"
		"obobbo3boboboo6b3ob3o44boo4bobo4bo$66bo10bo3boobbo4boo6bo45bobobobobob"
		"o9boo$67bo12bo8bobbo3boo65boo$76boobbobobb3o77bo$75bob4ob4obo$76bobobo"
		"bobobobbo62b3o$66b3o83bo$68bo84bo$67bo74bo$78bo62boo$78boo61bobo$77bob"
		"o$130boo$89boo39bobo$88bobo39bo$90bo$119boo$100boo16boo$101boo17bo$"
		"100bo$$111b3o$113bo$112bo20$179boo$178bobo$180bo21$246b3o$248bo$247bo!"
	};

	const ModelParameters newShuttle = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		51,
		51,
		"26b2o$20bo3bo2bo2bo$18b3o3b3o3b3o$8b2o7bo15bo7b2o$9bo7b2o5b3o5b2o7bo$"
		"9bobo11bo3bo11bobo$10b2o11b2ob2o11b2o2$3bo10b3o17b3o10bo$3b3o9bo19bo9b"
		"3o$6bo13b3o5b3o13bo$5b2o14bo7bo14b2o3$8bo33bo$8b2o31b2o$8bo33bo$3b2o"
		"18b2ob2o18b2o$2bobo18bo3bo18bobo$2bo21b3o21bo$b2o7bo15bobo11bo7b2o$10b"
		"2o15b2o10b2o$10bo9b2o18bo$2o3b2o10b2o2bo10b2o10b2o$obobobo10bob2o10bob"
		"o10bobob2o$2bobo14bo11bo14bobo$b2obobo10bobo10b2obo10bobobobo$5b2o10b"
		"2o10bo2b2o10b2o3b2o$10bo18b2o9bo$10b2o10b2o15b2o$b2o7bo11bobo15bo7b2o$"
		"2bo21b3o21bo$2bobo18bo3bo18bobo$3b2o18b2ob2o18b2o$8bo33bo$8b2o31b2o$8b"
		"o33bo3$5b2o14bo7bo14b2o$6bo13b3o5b3o13bo$3b3o9bo19bo9b3o$3bo10b3o17b3o"
		"10bo2$10b2o11b2ob2o11b2o$9bobo11bo3bo11bobo$9bo7b2o5b3o5b2o7bo$8b2o7bo"
		"15bo7b2o$18b3o3b3o3b3o$20bo2bo2bo3bo$23b2o!"
	};

	const ModelParameters pTwentyEightGliderShuttle = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		42,
		42,
		"25b2o$17b2o5bob2o2b2o$17b2o5bobo3b2o$24bo$22bo2$22bo$24bo$17b2o5bobo3b"
		"2o$17b2o5bob2o2b2o$b2o5b2o15b2o$b2o5b2o3$bo7bo$3o5b3o$o9bo5b2o5bobo$b"
		"3o3b3o7b2o5b2o6b2o5b2o$16bo7bo7b2o5b2o$4bobo3$35bobo$b2o5b2o7bo7bo$b2o"
		"5b2o6b2o5b2o7b3o3b3o$16bobo5b2o5bo9bo$31b3o5b3o$32bo7bo3$32b2o5b2o$15b"
		"2o15b2o5b2o$10b2o2b2obo5b2o$10b2o3bobo5b2o$17bo$19bo2$19bo$17bo$10b2o"
		"3bobo5b2o$10b2o2b2obo5b2o$15b2o!"
	};

	const ModelParameters pFourtySevenBHeptominoHassler = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		83,
		65,
		"12bo4bo47bo4bo$10b3o4b3o43b3o4b3o$9bo10bo41bo10bo$9b2o8b2o41b2o8b2o3$"
		"6b3o12b3o35b3o12b3o$14b2o51b2o$14b2o51b2o2$bo9bo6bo9bo25bo9bo6bo9bo$ob"
		"o7bo8bo7bobo23bobo7bo8bo7bobo$bo8bo8bo8bo25bo8bo8bo8bo$10bo8bo43bo8bo$"
		"11bo6bo45bo6bo2$11bo6bo45bo6bo$10bo8bo43bo8bo$bo8bo8bo8bo25bo8bo8bo8bo"
		"$obo7bo8bo7bobo23bobo7bo8bo7bobo$bo9bo6bo9bo25bo9bo6bo9bo2$14b2o51b2o$"
		"14b2o21bo7bo21b2o$6b3o12b3o13b3o3b3o13b3o12b3o$38bobobobo$39b2ob2o$9b"
		"2o7b2o43b2o7b2o$9bo8b2o10bob3o13b3obo10b2o8bo$10b3o19b2obo11bob2o19b3o"
		"$12bo19b2o15b2o19bo4$12bo19b2o15b2o19bo$10b3o19b2obo11bob2o19b3o$9bo8b"
		"2o10bob3o13b3obo10b2o8bo$9b2o7b2o43b2o7b2o$39b2ob2o$38bobobobo$6b3o12b"
		"3o13b3o3b3o13b3o12b3o$14b2o21bo7bo21b2o$14b2o51b2o2$bo9bo6bo9bo25bo9bo"
		"6bo9bo$obo7bo8bo7bobo23bobo7bo8bo7bobo$bo8bo8bo8bo25bo8bo8bo8bo$10bo8b"
		"o43bo8bo$11bo6bo45bo6bo2$11bo6bo45bo6bo$10bo8bo43bo8bo$bo8bo8bo8bo25bo"
		"8bo8bo8bo$obo7bo8bo7bobo23bobo7bo8bo7bobo$bo9bo6bo9bo25bo9bo6bo9bo2$"
		"14b2o51b2o$14b2o51b2o$6b3o12b3o35b3o12b3o3$9b2o8b2o41b2o8b2o$9bo10bo"
		"41bo10bo$10b3o4b3o43b3o4b3o$12bo4bo47bo4bo!"
	};

	const ModelParameters prePulsarShuttle26 = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		37,
		37,
		"16bo3bo16b$10b2o4bo3bo4b2o10b$10bo5bo3bo5bo10b$7b2obo15bob2o7b$6bobob"
		"2o13b2obobo6b$6bobo6bo5bo6bobo6b$4b2o2bo5b3o3b3o5bo2b2o4b$3bo4b2o17b2o"
		"4bo3b$3b5o21b5o3b$7bo21bo7b$b4o27b4ob$bo2bo27bo2bob2$15b2o3b2o15b$6bo"
		"9bo3bo9bo6b$5b2o6bo9bo6b2o5b$3o3bo6b2o7b2o6bo3b3o4$3o3bo6b2o7b2o6bo3b"
		"3o$5b2o6bo9bo6b2o5b$6bo9bo3bo9bo6b$15b2o3b2o15b2$bo2bo27bo2bob$b4o27b"
		"4ob$7bo21bo7b$3b5o21b5o3b$3bo4b2o17b2o4bo3b$4b2o2bo5b3o3b3o5bo2b2o4b$"
		"6bobo6bo5bo6bobo6b$6bobob2o13b2obobo6b$7b2obo15bob2o7b$10bo5bo3bo5bo"
		"10b$10b2o4bo3bo4b2o10b$16bo3bo!"
	};

	const ModelParameters ringOfFire = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		34,
		30,
		"16bo17b$14bobobo15b$12bobobobobo13b$10bobobobobobobo11b$8bobobo2b2obob"
		"obobo9b$6bobobobo6bo2bobobo7b$4bobobo2bo10bobobobo5b$5b2obo14bo2bobobo"
		"3b$3bo3bo18bob2o4b$4b3o20bo3bo2b$2bo25b3o3b$3b2o27bob$bo3bo24b2o2b$2b"
		"4o23bo3bo$o29b3ob$b3o29bo$o3bo23b4o2b$2b2o24bo3bob$bo27b2o3b$3b3o25bo"
		"2b$2bo3bo20b3o4b$4b2obo18bo3bo3b$3bobobo2bo14bob2o5b$5bobobobo10bo2bob"
		"obo4b$7bobobo2bo6bobobobo6b$9bobobobob2o2bobobo8b$11bobobobobobobo10b$"
		"13bobobobobo12b$15bobobo14b$17bo!"
	};

	const ModelParameters sirRobbin = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		31,
		79,
		"4b2o$4bo2bo$4bo3bo$6b3o$2b2o6b4o$2bob2o4b4o$bo4bo6b3o$2b4o4b2o3bo$o9b"
		"2o$bo3bo$6b3o2b2o2bo$2b2o7bo4bo$13bob2o$10b2o6bo$11b2ob3obo$10b2o3bo2b"
		"o$10bobo2b2o$10bo2bobobo$10b3o6bo$11bobobo3bo$14b2obobo$11bo6b3o2$11bo"
		"9bo$11bo3bo6bo$12bo5b5o$12b3o$16b2o$13b3o2bo$11bob3obo$10bo3bo2bo$11bo"
		"4b2ob3o$13b4obo4b2o$13bob4o4b2o$19bo$20bo2b2o$20b2o$21b5o$25b2o$19b3o"
		"6bo$20bobo3bobo$19bo3bo3bo$19bo3b2o$18bo6bob3o$19b2o3bo3b2o$20b4o2bo2b"
		"o$22b2o3bo$21bo$21b2obo$20bo$19b5o$19bo4bo$18b3ob3o$18bob5o$18bo$20bo$"
		"16bo4b4o$20b4ob2o$17b3o4bo$24bobo$28bo$24bo2b2o$25b3o$22b2o$21b3o5bo$"
		"24b2o2bobo$21bo2b3obobo$22b2obo2bo$24bobo2b2o$26b2o$22b3o4bo$22b3o4bo$"
		"23b2o3b3o$24b2ob2o$25b2o$25bo2$24b2o$26bo!"
	};

	const ModelParameters slowPuffer2 = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		500,
		119,
		"143bo$40bo102bobo$41bo101b2o$39b3o6$72bo$73b2o$72b2o5$56bo$57b2o$56b2o"
		"87bo$143b2o$144b2o2$53bo$54bo$52b3o22$14bo$3bobo7bo$4b2o7b3o$4bo5$bo$b"
		"2o$obo$11bo24bo$12b2o22bobo$11b2o23b2o19$72b2o$71bobo$8b2o63bo$9b2o$8b"
		"o17$152bo$139b3o9b2o$139bo11bobo$140bo7$55b3o86b2o$57bo85b2o$56bo88bo"
		"5$35b3o$37bo$36bo!"
	};

	const ModelParameters tNosedP8 = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		25,
		25,
		"6b2o3b3o3b2o$7bo4bo4bo2$6b13o$5bo13bo$4bo15bo$o2bo17bo2bo$2obo5bobobobo5bob2o"
		"$3bo5bobobobo5bo$3bo3b2obo3bob2o3bo$3bo5bo5bo5bo$o2bo3b2o7b2o3bo2bo$2obo17bob"
		"2o$o2bo3b2o7b2o3bo2bo$3bo5bo5bo5bo$3bo3b2obo3bob2o3bo$3bo5bobobobo5bo$2obo5bo"
		"bobobo5bob2o$o2bo17bo2bo$4bo15bo$5bo13bo$6b13o2$7bo4bo4bo$6b2o3b3o3b2o!"
	};

	const ModelParameters vexStabilisation = {
		false,
		-1,
		-1,
		-1,
		2,
		3,
		3,
		145,
		135,
		"40bo63bo$34b2o3bobo61bobo3b2o$35bo3bo2bo59bo2bo3bo$35bob2obobo59bobob"
		"2obo$36bobobob2o57b2obobobo$31b2o4b2obobo2bo53bo2bobob2o4b2o$32bo5bob"
		"2o2b2o53b2o2b2obo5bo$32bobo3b3o63b3o3bobo$33b2o3b2o65b2o3b2o5$33b3o73b"
		"3o$35bo73bo$34bo75bo$57bo29bo$55b3o29b3o$40bo13bo35bo13bo$39b3o12b2o33b"
		"2o12b3o$38bo2bo61bo2bo$38b3o9bo43bo9b3o$39bo8b3o43b3o8bo$23b2o7b2o13b"
		"3o6bo31bo6b3o13b2o7b2o$24b2o6bobo18b3ob2o27b2ob3o18bobo6b2o$23bo9bo13b"
		"2o2b2obo2bo29bo2bob2o2b2o13bo9bo$47b2o6b2o31b2o6b2o$47b2o2bo2bo35bo2b"
		"o2b2o$48b2o3bo37bo3b2o$49b3o41b3o$49b3o41b3o$44b2o11b2o27b2o11b2o$43b"
		"obo11bobo25bobo11bobo$13b2o28bo15bo25bo15bo28b2o$12bobo27b2o15b2o23b2o"
		"15b2o27bobo$14bo115bo2$46bo51bo$2o15b2o27bobo47bobo27b2o15b2o$bo15bo28b"
		"2o49b2o28bo15bo$bobo11bobo109bobo11bobo$2b2o11b2o111b2o11b2o$9b3o121b"
		"3o$9b3o121b3o$7bo3b2o119b2o3bo$6bo2bo2b2o117b2o2bo2bo$4b2o6b2o117b2o6b"
		"2o$3bo2bob2o2b2o13bo9bo69bo9bo13b2o2b2obo2bo$2b2ob3o18bobo6b2o71b2o6b"
		"obo18b3ob2o$4bo6b3o13b2o7b2o69b2o7b2o13b3o6bo$10b3o8bo101bo8b3o$10bo9b"
		"3o99b3o9bo$19bo2bo99bo2bo$5b2o12b3o101b3o12b2o$6bo13bo103bo13bo$3b3o133b"
		"3o$3bo137bo$26bo13bo31bo31bo13bo$25bo14bo31bo31bo14bo$25b3o12bo31bo31b"
		"o12b3o4$36bo7bo23bo7bo23bo7bo$35bobo5bobo21bobo5bobo21bobo5bobo$34bo3b"
		"o3bo3bo19bo3bo3bo3bo19bo3bo3bo3bo$35bobo5bobo21bobo5bobo21bobo5bobo2$"
		"35bobo5bobo21bobo5bobo21bobo5bobo$34bo3bo3bo3bo19bo3bo3bo3bo19bo3bo3b"
		"o3bo$35bobo5bobo21bobo5bobo21bobo5bobo$36bo7bo23bo7bo23bo7bo4$25b3o12b"
		"o31bo31bo12b3o$25bo14bo31bo31bo14bo$26bo13bo31bo31bo13bo$3bo137bo$3b3o"
		"133b3o$6bo13bo103bo13bo$5b2o12b3o101b3o12b2o$19bo2bo99bo2bo$10bo9b3o99b"
		"3o9bo$10b3o8bo101bo8b3o$4bo6b3o13b2o7b2o69b2o7b2o13b3o6bo$2b2ob3o18bo"
		"bo6b2o71b2o6bobo18b3ob2o$3bo2bob2o2b2o13bo9bo69bo9bo13b2o2b2obo2bo$4b"
		"2o6b2o117b2o6b2o$6bo2bo2b2o117b2o2bo2bo$7bo3b2o119b2o3bo$9b3o121b3o$9b"
		"3o121b3o$2b2o11b2o111b2o11b2o$bobo11bobo109bobo11bobo$bo15bo28b2o49b2o"
		"28bo15bo$2o15b2o27bobo47bobo27b2o15b2o$46bo51bo2$14bo115bo$12bobo27b2o"
		"15b2o23b2o15b2o27bobo$13b2o28bo15bo25bo15bo28b2o$43bobo11bobo25bobo11b"
		"obo$44b2o11b2o27b2o11b2o$49b3o41b3o$49b3o41b3o$48b2o3bo37bo3b2o$47b2o"
		"2bo2bo35bo2bo2b2o$47b2o6b2o31b2o6b2o$23bo9bo13b2o2b2obo2bo29bo2bob2o2b"
		"2o13bo9bo$24b2o6bobo18b3ob2o27b2ob3o18bobo6b2o$23b2o7b2o13b3o6bo31bo6b"
		"3o13b2o7b2o$39bo8b3o43b3o8bo$38b3o9bo43bo9b3o$38bo2bo61bo2bo$39b3o12b"
		"2o33b2o12b3o$40bo13bo35bo13bo$55b3o29b3o$57bo29bo$34bo75bo$35bo73bo$33b"
		"3o73b3o5$33b2o3b2o65b2o3b2o$32bobo3b3o63b3o3bobo$32bo5bob2o2b2o53b2o2b"
		"2obo5bo$31b2o4b2obobo2bo53bo2bobob2o4b2o$36bobobob2o57b2obobobo$35bob"
		"2obobo59bobob2obo$35bo3bo2bo59bo2bo3bo$34b2o3bobo61bobo3b2o$40bo63bo!"
	};

}




#endif //GAMEOFLIFE_MODELPRESETS_H
