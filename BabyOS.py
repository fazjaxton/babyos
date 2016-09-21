import sfml as sf
import os
from GameObject import GameObject
import math
import random
import ImageCache
import Animation

keymap = {
    sf.Keyboard.A: "a",
    sf.Keyboard.B: "b",
    sf.Keyboard.C: "c",
    sf.Keyboard.D: "d",
    sf.Keyboard.E: "e",
    sf.Keyboard.F: "f",
    sf.Keyboard.G: "g",
    sf.Keyboard.H: "h",
    sf.Keyboard.I: "i",
    sf.Keyboard.J: "j",
    sf.Keyboard.K: "k",
    sf.Keyboard.L: "l",
    sf.Keyboard.M: "m",
    sf.Keyboard.N: "n",
    sf.Keyboard.O: "o",
    sf.Keyboard.P: "p",
    sf.Keyboard.Q: "q",
    sf.Keyboard.R: "r",
    sf.Keyboard.S: "s",
    sf.Keyboard.T: "t",
    sf.Keyboard.U: "u",
    sf.Keyboard.V: "v",
    sf.Keyboard.W: "w",
    sf.Keyboard.X: "x",
    sf.Keyboard.Y: "y",
    sf.Keyboard.Z: "z",
    sf.Keyboard.NUM0: "0",
    sf.Keyboard.NUM1: "1",
    sf.Keyboard.NUM2: "2",
    sf.Keyboard.NUM3: "3",
    sf.Keyboard.NUM4: "4",
    sf.Keyboard.NUM5: "5",
    sf.Keyboard.NUM6: "6",
    sf.Keyboard.NUM7: "7",
    sf.Keyboard.NUM8: "8",
    sf.Keyboard.NUM9: "9",
}

class Picture(GameObject):
    def __init__(self, texture, application):
        GameObject.__init__(self)
        self.sprite = sf.Sprite(texture)
        self.addChild(self.sprite)
        self.application = application
        x = texture.width / 2
        y = texture.height / 2
        self.origin = sf.Vector2(texture.width / 2, texture.height / 2)
        self.animations = Animation.createRandomGroup(self)

    def frameUpdate(self):
        if self.animations.complete:
            self.application.addToBackground(self.sprite.texture, self.position - self.origin)
            self.application.removeObject(self)
        else:
            self.animations.update()

class Scene(object):
    def __init__(self):
        self.objects = []

    def add(self, o):
        self.objects.append(o)

    def remove(self, o):
        if o in self.objects:
            self.objects.remove(o)

    def each(self):
        for o in self.objects:
            yield o

class BabyOS:
    def __init__(self):
        mode = sf.VideoMode.get_desktop_mode()
        style = sf.Style.FULLSCREEN
        
        self.window = sf.RenderWindow(mode, "BabyOS", style)
        Animation.setScreenSize(mode.size)
        
        self.bgImage = sf.Image.create(mode.size.x, mode.size.y, sf.Color.BLACK)
        bgTexture = sf.Texture.from_image(self.bgImage)
        self.bgSprite = sf.Sprite(bgTexture)

        self.scene = Scene()
        self.lettermap = {}
        self.codeword = "stop"
        self.typed = ""

        ic = ImageCache.ImageCache(maxDim=400)
        ic.scan("pictures")

        for i in ic.getCachedImages():
            try:
                texture = sf.Texture.from_file(i)
            except:
                print "Error loading texture %s" % (i)
                continue

            key = os.path.basename(i)[0]
            key = key.lower()
            if not key in self.lettermap:
                self.lettermap[key] = []
            self.lettermap[key].append(texture)

    def getPosAndRect(self, texture, position):
        # Some images have their center onscreen, but their top-left corner
        # offscreen.  Image.blit is going to be upset if it gets a negative
        # position value, so move the position to 0 and use a sub-rectangle of the
        # source image to create the same effect.
        x, y = position
        l, t = (0, 0)
        w, h = texture.size

        if x < 0:
            w += x
            l -= x
            x = 0
        if y < 0:
            h += y
            t -= y
            y = 0

        return (sf.Vector2(x,y), sf.Rectangle(sf.Vector2(l, t), sf.Vector2(w, h)))

    def addToBackground(self, texture, position):
        i = texture.to_image()
        p, r = self.getPosAndRect(texture, position)

        self.bgImage.blit(i, p, r)
        self.bgSprite.texture = sf.Texture.from_image(self.bgImage)

    def addLetterToCode(self, letter):
        self.typed += letter
        self.typed = self.typed[-len(self.codeword):]
        if self.typed == self.codeword:
            self.window.close()

    def handleKey(self, code):
        global keymap
        if code in keymap:
            letter = keymap[code]
            if letter in self.lettermap:
                tex = random.choice(self.lettermap[letter])
                p = Picture(tex, self)
                self.scene.add(p)
            self.addLetterToCode(letter)

    def removeObject(self, obj):
        self.scene.remove(obj)

    def run(self):
        while (self.window.is_open):
            for event in self.window.events:
                if event == sf.CloseEvent:
                    self.window.close()
                if event == sf.KeyEvent:
                    if event.pressed:
                        self.handleKey(event.code)

            for o in self.scene.each():
                o.frameUpdate()

            self.window.clear()

            self.window.draw(self.bgSprite)
            for o in self.scene.each():
                self.window.draw(o)

            self.window.display()

if __name__ == "__main__":
    babyos = BabyOS()
    babyos.run()
