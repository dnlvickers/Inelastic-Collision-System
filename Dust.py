import random

#Here are all of the convenient math funciotns needed
def vectorSum(v1,v2):
	vx = v1[0]+v2[0]
	vy = v1[1]+v2[1]
	v = [vx,vy]
	return v

def vectorMult(v1,s):
	vx = v1[0]*s
	vy = v1[1]*s
	v = [vx,vy]
	return v

#We define what a piece of "dust" will look lik
class Dust:

	#Define useful properties
	position = []
	momentum = []
	mass = 1
	radius = 1
	ID = 1

	def __init__(self, position, momentum, mass, ident):
		self.position = position
		self.momentum = momentum
		self.mass = mass
		self.radius = mass**(1/3)
		self.ID = ident

	def getPosition(self):
		return self.position

	def getMomentum(self):
		return self.momentum

	def setMomentum(self, vec):
		self.momentum = vec

	def getMass(self):
		return self.mass

	def getRadius(self):
		return self.radius

	def getID(self):
		return self.ID

	def step(self, dt):
		i = 0
		while i < 2:
			if self.mass == 0:
				print("Error: Particle ID " + str(self.ID) + " has no mass")
				break
			else:
				self.position[i] = self.position[i] + ((self.momentum[i]/self.mass) * dt)
			i += 1

	def collide(self, d2):
		self.mass = self.mass + d2.getMass
		self.position = vectorMult(vectorSum(self.position,d2.getPosition),0.5)
		self.momentum = vectorSum(self.momentum,d2.getMomentum)
		self.radius = mass**(1/3)