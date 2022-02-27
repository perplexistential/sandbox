#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


/*
// SpriteSheetConfig describes a SpriteSheet
struct SpriteSheetConfig {
 RRring                        	Name
 map[string]PaletteConfig      	Palettes
 []SpriteSeriesConfig          	Sprites
 string `yaml:"defaultPalette"`	DefaultPalette
}

// PaletteConfig describes a color palette
type PaletteConfig struct {
        X int `yaml:"x"`
        Y int `yaml:"y"`
        W int `yaml:"width"`
        H int `yaml:"height"`
}

// SpriteSeriesConfig describes a row in the SpriteSheet
type SpriteSeriesConfig struct {
        Name            string
        X               int
        Y               int
        Padding         int `yaml:"padding"`
        Count           int `yaml:"count"`
        SpriteWidth     int `yaml:"spriteWidth"`
        SpriteHeight    int `yaml:"spriteHeight"`
        CyclesPerSecond int `yaml:"cyclesPerSecond"`
        StartIndex      int
        Palette         string
}

type SpriteSheet struct {
        File    string
        Sheet   [][]Frame
        Sprites map[string]Sprite
}

type SpriteCoord struct {
        X, Y int
}

type Sprite struct {
        CyclesPerSecond int
        Coords          []SpriteCoord
}

type Frame struct {
        X, Y          int
        Width, Height int
}

func loadSprite(spriteName string, ss *SpriteSheet) {
        inputFile := path.Join("assets", spriteName, spriteName+".png")
        configFile := path.Join("assets", spriteName, spriteName+".yaml")
        ssc, err := loadSpriteSheetConfig(configFile)
        if err != nil {
                log.Fatal(fmt.Sprintf("Failed loading sprite sheet %s: %v\n",
configFile, err))
        }
        ss.File = inputFile
        ss.Sheet = make([][]Frame, len(ssc.Sprites))
        for j, seriesCfg := range ssc.Sprites {
                coords := []SpriteCoord{}
                ss.Sheet[j] = []Frame{}
                i := 0
                // Iterate along a single row
                for x := seriesCfg.X; x <
seriesCfg.X+seriesCfg.SpriteWidth*seriesCfg.Count; x += seriesCfg.SpriteWidth {
                        frame := Frame{
                                X: x, Y: seriesCfg.Y,
                                Width:  seriesCfg.SpriteWidth,
                                Height: seriesCfg.SpriteHeight,
                        }
                        fmt.Printf("%v: (%v,%v) - %v\n", seriesCfg.Name, i, j,
frame)
                        // fmt.Printf("sprite frame: %v\n", frame)
                        ss.Sheet[j] = append(ss.Sheet[j], frame)
                        coords = append(coords, SpriteCoord{X: i, Y: j})
                        i += 1
                }
                newSprite := Sprite{
                        Coords:          coords,
                        CyclesPerSecond: seriesCfg.CyclesPerSecond,
                }
                ss.Sprites[seriesCfg.Name] = newSprite
                // fmt.Printf("sprite: %v\n", newSprite)
        }
}

func loadSpriteSheetConfig(configFile string) (*SpriteSheetConfig, error) {
        if len(configFile) == 0 {
                log.Fatal("no sprite config, no cry")
        }
        // Read the config file
        cfgBytes, err := ioutil.ReadFile(configFile)
        if err != nil {
                return nil, fmt.Errorf("error reading input file")
        }
        cfg := SpriteSheetConfig{}
        if err = yaml.Unmarshal(cfgBytes, &cfg); err != nil {
                log.Fatalf("Error unmarshaling image config file: %v", err)
        }
        return &cfg, nil
}

type spriteState struct {
        Frame  int
        State  int
        Update float64
}
*/
