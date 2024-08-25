# scene.ne

CameraBegin
    width 800
    height 600
    pos [0,-5,0]
    look [0,1,0]
    up [0,0,1]
    fov 60
    focalLength 1
    lenRadius 0.1
CameraEnd

RenderOptionBegin
    maxDepth 100
    russianRoulete 0.8
    gamma 2.2
    accumulate true
RenderOptionEnd

SceneBegin
    ObjectBegin # Left Wall
        ShapeBegin
            type Sphere
            origin [-1000,0,0]
            radius 999.3
            reverseNormal false
        ShapeEnd
        MaterialBegin
            type Lambertian
            kd [1,0,0]
            glowSpectrum [0,0,0]
        MaterialEnd
    ObjectEnd
    ObjectBegin # Right Wall
        ShapeBegin
            type Sphere
            origin [1000,0,0]
            radius 999.3
            reverseNormal false
        ShapeEnd
        MaterialBegin
            type Lambertian
            kd [0,1,0]
            glowSpectrum [0,0,0]
        MaterialEnd
    ObjectEnd
    ObjectBegin # Upper Wall
        ShapeBegin
            type Sphere
            origin [0,0,1000]
            radius 999.5
            reverseNormal false
        ShapeEnd
        MaterialBegin
            type Lambertian
            kd [1,1,1]
            glowSpectrum [0,0,0]
        MaterialEnd
    ObjectEnd
    ObjectBegin # Bottom Wall
        ShapeBegin
            type Sphere
            origin [0,0,-1000]
            radius 999.5
            reverseNormal false
        ShapeEnd
        MaterialBegin
            type Lambertian
            kd [1,1,1]
            glowSpectrum [0,0,0]
        MaterialEnd
    ObjectEnd
    ObjectBegin # Inside Wall
        ShapeBegin
            type Sphere
            origin [0,1000,0]
            radius 999
            reverseNormal false
        ShapeEnd
        MaterialBegin
            type Lambertian
            kd [1,1,1]
            glowSpectrum [0,0,0]
        MaterialEnd
    ObjectEnd
    ObjectBegin # Glowing Sphere
        ShapeBegin
            type Sphere
            origin [0.2,-1,-0.3]
            radius 0.2
            reverseNormal false
        ShapeEnd
        MaterialBegin
            type Lambertian
            kd [1,1,1]
            glowSpectrum [1,1,1]
        MaterialEnd
    ObjectEnd
SceneEnd
