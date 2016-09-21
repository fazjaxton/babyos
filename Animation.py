import sfml as sf
import math
import random


offScreenMax = 100

randRanges = {
    "duration": (1.0, 3.0),
    "spins":    (1,5),
    "zoom":     (1.0, 2.0),

}

randPops = {
    "edges":    ("top", "bottom", "left", "right"),
    "spindir":  (-1, 1),
}

def setScreenSize(size):
    global randRanges
    randRanges["screenx"]   = (0, size.x)
    randRanges["screeny"]   = (0, size.y)

    randRanges["topx"]      = (0, size.x)
    randRanges["bottomx"]   = (0, size.x)
    randRanges["leftx"]     = (-offScreenMax, 0)
    randRanges["rightx"]    = (0, offScreenMax)

    randRanges["topy"]      = (-offScreenMax, 0)
    randRanges["bottomy"]   = (0, offScreenMax)
    randRanges["lefty"]     = (0, size.y)
    randRanges["righty"]    = (0, size.y)


def _randVal(t):
    if t in randRanges:
        r = randRanges[t]
        if type(r[0]) is int:
            return random.randrange(*r)
        elif type(r[0]) is float:
            return random.uniform(*r)
    if t in randPops:
        return random.choice(randPops[t])


def _randOnScreen():
    x = _randVal("screenx")
    y = _randVal("screeny")
    return sf.Vector2(x, y)

def _randOffScreen():
    offEdge = _randVal("edges")
    x = _randVal(offEdge + "x")
    y = _randVal(offEdge + "y")

    return sf.Vector2(x, y)

class BaseAnimation(object):                
    def __init__(self, target):
        self.target = target
        self.startTime = _now()
        self.duration = 0

    def setDuration(self, duration):
        self.duration = duration

    def randomize(self):
        pass

    def update(self, time, percent):
        pass

    def final(self):
        pass


class PositionAnimation(BaseAnimation):
    def __init__(self, target):
        BaseAnimation.__init__(self, target)
        self.randomize()

    def randomize(self):
        BaseAnimation.randomize(self)
        self.startPos = _randOffScreen()
        self.endPos = _randOnScreen()
        self.vector = self.endPos - self.startPos

    def final(self):
        self.target.position = self.endPos


class AppearAnimation(PositionAnimation):
    def __init__(self, target):
        PositionAnimation.__init__(self, target)
        self.duration = 0

    def setDuration(self, duration):
        # Always 0 duration
        pass

class LinearAnimation(PositionAnimation):
    def __init__(self, target):
        PositionAnimation.__init__(self, target)

    def update(self, time, percent):
        self.target.position = self.vector * percent + self.startPos


class SpiralAnimation(PositionAnimation):
    def __init__(self, target):
        PositionAnimation.__init__(self, target)
        self.dir = 1
        self.mag = math.sqrt(self.vector.x ** 2 + self.vector.y ** 2)
        self.startAngle = math.atan2(-self.vector.y, -self.vector.x)

    def update(self, time, percent):
        rad = self.mag * (1 - percent)
        theta = self.startAngle + 2 * math.pi * percent
        x = rad * math.cos(theta)
        y = rad * math.sin(theta)
        self.target.position = sf.Vector2(x,y) + self.endPos


class ScaleAnimation(BaseAnimation):
    def __init__(self, target):
        BaseAnimation.__init__(self, target)
        self.baseZoom = target.ratio

    def final(self):
        self.target.ratio = self.baseZoom


class NoScaleAnimation(ScaleAnimation):
    def __init__(self, target):
        ScaleAnimation.__init__(self, target)
        self.duration = 0

    def setDuration(self, duration):
        # Always 0 duration
        pass


class InAndOutAnimation(ScaleAnimation):
    def __init__(self, target):
        ScaleAnimation.__init__(self, target)
        self.randomize()

    def randomize(self):
        ScaleAnimation.randomize(self)
        self.maxZoom = _randVal("zoom")
        self.zoomsPerSecond = 2

    def update(self, time, percent):
        s = math.sin(percent * self.zoomsPerSecond * math.pi)
        scale = self.maxZoom ** s
        self.target.ratio = sf.Vector2(1,1) * scale * self.baseZoom.x


class RotationAnimation(BaseAnimation):
    def __init__(self, target):
        BaseAnimation.__init__(self, target)
        self.startRotation = self.target.rotation

    def final(self):
        self.target.rotation = self.startRotation


class SpinAnimation(RotationAnimation):
    def __init__(self, target):
        RotationAnimation.__init__(self, target)
        self.randomize()

    def randomize(self):
        RotationAnimation.randomize(self)
        self.dir = _randVal("spindir")
        self.spins = _randVal("spins")

    def update(self, time, percent):
        r = self.dir * self.spins * 360 * percent + self.startRotation
        self.target.rotation = r


class NoRotationAnimation(RotationAnimation):
    def __init__(self, target):
        RotationAnimation.__init__(self, target)
        self.duration = 0

    def setDuration(self, duration):
        # Always 0 duration
        pass


class AnimationGroup(object):
    def __init__(self, duration):
        self.animations = []
        self.duration = duration
        self.complete = False

    def add(self, a):
        a.setDuration(self.duration)
        self.animations.append(a)

    def update(self):
        complete = []

        for a in self.animations:
            time = _now() - a.startTime
            if a.duration > 0:
                percent = time / a.duration
            else:
                percent = 1.0
            if percent >= 1.0:
                complete.append(a)
            else:
                a.update(time, percent)

        for a in complete:
            a.final()
            self.animations.remove(a)

        if len(self.animations) == 0:
            self.complete = True


_clock = sf.Clock()

_positionAnimations = [AppearAnimation, LinearAnimation, SpiralAnimation]
_scaleAnimations = [NoScaleAnimation, InAndOutAnimation]
_rotationAnimations = [NoRotationAnimation, SpinAnimation]

def _now():
    return _clock.elapsed_time.seconds

def createRandomGroup(target):
    duration = _randVal("duration")
    g = AnimationGroup(duration)
    g.add(random.choice(_positionAnimations)(target))
    g.add(random.choice(_scaleAnimations)(target))
    g.add(random.choice(_rotationAnimations)(target))
    return g
