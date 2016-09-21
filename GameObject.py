import sfml as sf

class GameObject (sf.TransformableDrawable):
    def __init__ (self):
        sf.TransformableDrawable.__init__(self)
        self.velocity = sf.Vector2(0, 0)
        self.children = []
        self.collider = None

    def physUpdate (self, dt):
        pass

    def frameUpdate (self, dt):
        pass

    def draw (self, target, states):
        self.drawSelf (target, states)

        states.transform *= self.transform
        for child in self.children:
            target.draw (child, states)

    def drawSelf (self, target, states):
        pass

    def onCollide(self, collider):
        pass

    def addChild (self, child):
        self.children.append (child)
