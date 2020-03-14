from Dust import Dust
import random

#Usefule functions
def magnitude(vec):
	mag = ((vec[0]*vec[0])+(vec[1]*vec[1]))**(0.5)
	return(mag)

def vectorSum(v1,v2):
	vx = v1[0]+v2[0]
	vy = v1[1]+v2[1]
	v = [vx,vy]
	return v

def vectorSubtract(v1,v2):
	vx = v1[0]-v2[0]
	vy = v1[1]-v2[1]
	v = [vx,vy]
	return v

def vectorMult(v1,s):
	vx = v1[0]*s
	vy = v1[1]*s
	v = [vx,vy]
	return v

def spin(r,G,m):
	mag = magnitude(r)
	perp = [-r[1],r[0],0]
	vel = vectorMult(perp,1/mag)
	vel = vectorMult(vel,(G*m/mag)**(0.5))
	return vel

def crossMag(v,u):
	cross = v[0]*u[1] - v[1]*u[0]
	return cross

#magnitude of gravitational force on d2 by d1
def fG(d1,d2,G):
	r1 = d1.getPosition()
	r2 = d2.getPosition()
	m1 = d1.getMass()
	m2 = d2.getMass()
	diff = vectorSubtract(r2,r1)
	r = magnitude(diff)

	F = vectorMult(diff,-G*m1*m2/(r**3))
	return F

def writeData(particleList,time):
	i = 0
	while i < len(particleList):
		p = particleList[i]
		pos = p.getPosition()
		mom = p.getMomentum()
		dataFile.write(str(p.getID()) + "," + str(time)+ "," + str(pos[0]) + "," + str(pos[1]) + "," + str(p.getMass()) + "\n")
		i += 1

#And God (me in this case) said, "let there be dust"...

N1 = 6000
N2 = 4000
G = 6.674*(10**(-11))
starMass = 10**30
R = 70*(10**9)
massrange = 1 * (10**23)
star = Dust([0,0],[0,0],starMass,"star")
particles = []

dataFile = open("positions.csv", "w")
dataFile.write("id,time,x,y,m" + "\n")

angularFile = open("angular.csv", "w")
angularFile.write("time,L\n")



i = 0
while i  < N1:
	magr = 0
	while (magr < 46*(10**9)) or (magr > R): #first, we generate a random position within our anulus
		x = (random.random() - 0.5) * 2 * R
		y = (random.random() - 0.5) * 2 * R
		r = [x,y]
		magr = magnitude(r)

	#Now we generate a random momentum
	px = (random.random() - 0.5) * 0.4 * (G*starMass/magr)**(1/2)
	py = (random.random() - 0.5) * 0.4 * (G*starMass/magr)**(1/2)
	p = [px,py]

	p = vectorSum(p,spin(r,G,starMass))

	mass = (random.random() * 2 * massrange) + (massrange)

	p = vectorMult(p,mass)

	particles.append(Dust(r,p,mass,i)) #create our dust particle

	i += 1

i = 0
while i  < N2:
	magr = 0
	while (magr < 46*(10**9)) or (magr > R): #first, we generate a random position within our anulus
		x = (random.random() - 0.5) * 2 * R
		y = (random.random() - 0.5) * 2 * R
		r = [x,y]
		magr = magnitude(r)

	#Now we generate a random momentum
	px = (random.random() - 0.5) * 0.4 * (G*starMass/magr)**(1/2)
	py = (random.random() - 0.5) * 0.4 * (G*starMass/magr)**(1/2)
	p = [px,py]

	p = vectorSum(p,vectorMult(spin(r,G,starMass),-1))

	mass = (random.random() * 2 * massrange) + (massrange)

	p = vectorMult(p,mass)

	particles.append(Dust(r,p,mass,i)) #create our dust particle

	i += 1

print("Finished generating particles")


writeData(particles,0)

#And on the third day, God created gravity
#He actually needed to start time and everything here too. Don't ask how we have days without time. Just go with it.
t = 0
dt = 60*60*3
T = dt*80000
count = 0
collisions = 0
starCollisions = 0
inf = 0

while t < T:
	i = 0
	L = 0
	while i < len(particles):
		#First, calcualte the graviational force on all particles, then update positions accordingly
		F = fG(star,particles[i],G)
		j = 0
		while j < len(particles):
			if i == j:
				j += 1
			else:
				F = vectorSum(F,fG(particles[j],particles[i],G))
				j += 1
				
		particles[i].setMomentum(vectorSum(particles[i].getMomentum(),vectorMult(F,dt)))
		particles[i].step(dt)

		#And on the fourth day, God created collision detection, and it was good.
		j = 0
		compPos = particles[i].getPosition() #store useful radius values so that we don't have to constantly retreive them
		compRad = particles[i].getRadius()
		while j < i:
			if magnitude(vectorSubtract(compPos,particles[j].getPosition())) <= (compRad + particles[j].getRadius()) * 1.5:
				#Collide particles close enough to be inside of each other
				particles[i].collide(particles[j])
				collisions += 1
				print("Inelastic Collision No. " + str(collisions) + " at time fraction " + str(t / (T)))
				i -= 1
				#We no longer need element j. So, we delete it, and then continue to not incrament count
				del(particles[j])
				continue
			j += 1
		if magnitude(compPos) < star.getRadius():
			star.collide(particles[i])
			del(particles[i])
			starCollisions += 1
			print("Solar Collision No. " + str(starCollisions) + " at time fraction " + str(t / (T)))
			continue
		if  magnitude(compPos) > 10*R:
			del(particles[i])
			inf += 1
			print("Particle Flew Off to Infinity No. " + str(inf) + " at time fraction " + str(t/T))
			continue
		L += crossMag(compPos,particles[i].getMomentum())
		i += 1
	t += dt
	count += 1
	if count == 4:
		writeData(particles,t)
		angularFile.write(str(t)+","+str(L) + "\n")
		count = 0
	#print("Finished time step : " + str(t))

print("Inelastic Collisions = " + str(collisions))
print("Solar Collisions = " + str(starCollisions))
print("Particles at Infinity = " + str(inf))
print("Remaining particles = " + str(N1 + N2 - collisions - starCollisions - inf))