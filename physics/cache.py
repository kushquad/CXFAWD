PARTICLE_CACHE_FILE = "../cache/particle.cch"

class CacheExport:
	def __init__(self, cachefile=PARTICLE_CACHE_FILE):
		self.cachefile = cachefile

	def clearCache(self):
		file = open(self.cachefile,"w")
		file.close()

	def exportToCache(self, particledata):
		file = open(self.cachefile,"a")
		file.write(particledata)
		file.close()
