import imghdr
import Image
import hashlib
import os

class ImageCache(object):
    def __init__(self, cacheDir=None, maxDim=None):
        if cacheDir == None:
            cacheDir = os.path.join("imgcache")
        self.cacheDir = cacheDir
        if not os.path.isdir(cacheDir):
            os.mkdir(cacheDir)
        self.maxDim = maxDim
        self.updateCacheList()

    def getFileHash(self, name):
        stat = os.stat(name)
        basename = os.path.basename(name)

        h = hashlib.sha1()
        h.update(basename)
        h.update(str(stat.st_size))
        return h.hexdigest()

    def getCacheName(self, name):
        prefix = os.path.basename(name)[0].lower()
        return os.path.join(self.cacheDir, prefix + self.getFileHash(name))

    def isCached(self, name):
        return os.path.isfile(self.getCacheName(name))

    def isImage(self, name):
        return imghdr.what(name) != None

    def checkScale(self, img):
        if self.maxDim == None:
            return img
        w = img.size[0]
        h = img.size[1]
        div = 1
        while w / div > self.maxDim:
            div *= 2
        while h / div > self.maxDim:
            div *= 2

        if div == 1:
            return img

        newsize = (img.size[0] / div, img.size[1] / div)
        return img.resize(newsize, Image.ANTIALIAS)

    def updateCacheList(self):
        self.cacheList = []
        for entry in os.listdir(self.cacheDir):
            self.cacheList.append(os.path.join(self.cacheDir, entry))

    def cache(self, name):
        try:
            i = Image.open(name)
            i = self.checkScale(i)
            cacheName = self.getCacheName(name)
            i.save(cacheName, 'png')
            print "Cached %s to %s" % (name, cacheName)
            return True
        except IOError:
            print "Error caching %s" % name
            return False

    def scan(self, sourceDir):
        for entry in os.listdir(sourceDir):
            name = os.path.join(sourceDir, entry)
            print "checking %s" % name
            if os.path.isdir(name):
                print "  directory, skipping"
                continue

            if not self.isImage(name):
                print "  not image, skipping"
                continue

            if not self.isCached(name):
                print "  caching"
                self.cache(name)
            else:
                print "  already cached"

        self.updateCacheList()

    def getCachedImages(self):
        return self.cacheList

