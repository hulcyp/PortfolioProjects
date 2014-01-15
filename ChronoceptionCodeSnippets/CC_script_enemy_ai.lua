--#require CC_script_collision
dofile( self, APP_PATH .. "scripts/include/CC_script_audioIndex_defines.lua" )

--JS: File load and execution priority
priority = -950


--UI Variables
moveSpeed     = 0.0
initialDir    = "LEFT"
jumpSpeed     = 10.0
friction      = 0.9
health        = 1
healthRegen   = 0
armor         = 0
startFullArmor = true
armorRegen    = 0
lives         = 1
score         = 0
stunTime      = 0.75


--AI variables
initialized = false
isAlive     = false
maxHealth      = 1
currentHealth  = 0
useHealthRegen = false
healthRegen    = 0
maxArmor       = 1
currentArmor   = 0
startFullArmor = false
useArmorRegen  = false
armorRegen     = 0
maxLives       = 1
currentLives   = 0
score          = 0
damageType     = ""
damageToGive   = 0
damageReceived = 0
maxSpeed  = vec2( 0, 0 )
dir       = 1
newSpeed	= vec2( 0, 0 )
lastDirection = vec2( 0, 0 )
velocity  = vec2( 0, 0 )
rotation  = 0
friction  = 0
--facing    = "right"
frozenDuringCollision = false
maxStunTime = 0
currentStunTime = 0
deathRemoveTime = 0
checkTime = 0
isInCombat = false
aiDrawCollision = false

--PH: Varaibles specific for Chronoception

--Pathing variables
movementPath = "VERTICAL"

verticalPath = { vec2( 0.0, 1.0 ), vec2( 0.0, -1.0 ), nil }
veritcalPathTimes = { 1.0, 1.0, nil }

horizontalPath = { vec2( 1.0, 0.0 ), vec2( -1.0, 0.0 ), nil }
horizontalPathTimes = { 1.0, 1.0, nil }

trianglePath = { vec2( 1.0, 0.0 ), vec2( -0.5, -sqrt(0.75) ), vec2( -0.5, sqrt(0.75) ), nil }
trianglePathTimes = { 1.0, 1.0, 1.0, nil }

boxPath = { vec2( 1.0, 0.0 ), vec2( 0.0, 1.0 ), vec2( -1.0, 0.0 ), vec2( 0.0, -1.0 ), nil }
boxPathTimes = { 1.0, 1.0, 1.0, 1.0, nil }

horizontalBowTiePath = { vec2( 0.0, -1.0 ), vec2( 0.5*tan( radians( 75.0 ) ), 0.5 ), vec2( 0.0, -1.0 ), vec2( -0.5*tan( radians( 75.0 ) ), 0.5 ), nil }
horizontalBowTiePathTimes = { 1.0, 4.0, 1.0, 4.0, nil }

verticalBowTiePath = { vec2( 1.0, 0.0 ), vec2( -0.5, -0.5*tan( radians( 75.0 ) ) ), vec2( 1.0, 0.0 ), vec2( -0.5, 0.5*tan( radians( 75.0 ) ) ), nil }
verticalBowTiePathTimes = { 1.0, 4.0, 1.0, 4.0, nil }

starPath = { vec2( cos( radians( 72.0 ) ), -sin( radians( 72.0 ) ) ), vec2( cos( radians( 72.0 ) ), sin( radians( 72.0 ) ) ), vec2( -1.0, -tan( radians( 36 ) ) ), vec2( 1.0, 0.0 ), vec2( -1.0, tan( radians( 36.0 ) ) ) }
starPathTimes = { 1.0, 1.0, 1.0, 1.0, 1.0 }


--Used to keep track of which path the AI is currently on
currentPathIndex = 1
currentPath = { vec2( 0, 0 ), nil }
currentPathTimes = { 0.0 }

--Path properites
scaleOfPath = 1.0
startingPoint = 1
pathDir = -1
pathDirection = "CLOCKWISE"

--Keep track of the time on the current path
timeOnPath = 0.0

--Time manipuliation modifier
modifier = 0
timeModifier = 0

--For conteracting gravity
initialX = 0
initialY = 0
prevPos = vec2( 0, 0 )

damageToDeal = 1

--Chase player
doesChasePlayer = false
isChasingPlayer = false
chaseRadius = 10.0
newState = ""

--Collision sound effect
collisionSoundEffect = "harbingerHit"

chasingSoundEffect = "harbingerChase"
isPlayingChaseSE = false


ui = {
	moveSpeed      	= { order = 1,  type = "number",  label = "Movement Speed" },
	movementPath	= { order = 2,  type = "list", values = { "VERTICAL", "HORIZONTAL", "TRIANGLE", "BOX", "HORIZONTAL BOWTIE", "VERTICAL BOWTIE", "STAR PATH" }, label = "Movement Path", default = "VERTICAL" },
	scaleOfPath     = { order = 3,  type = "number",  label = "Path Scale" },
	startingPoint	= { order = 4,	type = "number", label = "Starting Position" },
	pathDirection	= { order = 5,	type = "list", values = { "CLOCKWISE", "COUNTERCLOCKWISE" }, label = "Path Direction" },
	damageToDeal   	= { order = 6, type = "number",  label = "Damage Dealt" },
	doesChasePlayer	= { order = 7, type = "bool", label = "Chases Player" },
	chaseRadius		= { order = 8, type = "number", label = "Chase distance" },
	aiDrawCollision = { order = 9, type = "bool", label = "Show collision" },
	collisionSoundEffect = { order = 10, type = "list", values = g.AM_indexList, label = "Collision Sound Effect" },
	chasingSoundEffect = { order = 11, type = "list", values = g.AM_indexList, label = "Chasing Sound Effect" },
}

local justDidDamage = true

local debugComponent = false
local devFont = Font( "arial", 16 )

function normalize( paths )
	local i = 1
	while( paths[i] ) do
		length = sqrt( paths[i].x*paths[i].x + paths[i].y*paths[i].y )
		paths[i].x = paths[i].x/length
		paths[i].y = paths[i].y/length
		i = i + 1
	end
end


function setPathing()
	
	scaleOfPath = g.math.clamp( scaleOfPath, 0.0, 20.0 )
	
	if( pathDirection == "CLOCKWISE" ) then
		pathDir = 1
	elseif( pathDirection == "COUNTERCLOCKWISE" ) then
		pathDir = -1
	end	
	
	if( movementPath == "VERTICAL" ) then
		currentPath = verticalPath		
		startingPoint = g.math.clamp( startingPoint, 1, 2 )
		currentPathIndex = startingPoint
		currentPathTimes = veritcalPathTimes
	elseif( movementPath == "HORIZONTAL" ) then
		currentPath = horizontalPath
		startingPoint = g.math.clamp( startingPoint, 1, 2 )
		currentPathIndex = startingPoint
		currentPathTimes = horizontalPathTimes
	elseif( movementPath == "TRIANGLE" ) then
		normalize( trianglePath )
		currentPath = trianglePath
		startingPoint = g.math.clamp( startingPoint, 1, 3 )
		currentPathIndex = startingPoint
		currentPathTimes = trianglePathTimes
	elseif( movementPath == "BOX" ) then
		currentPath = boxPath
		startingPoint = g.math.clamp( startingPoint, 1, 4 )
		currentPathIndex = startingPoint
		currentPathTimes = boxPathTimes
	elseif( movementPath == "HORIZONTAL BOWTIE" ) then
		normalize( horizontalBowTiePath )
		currentPath = horizontalBowTiePath
		currentPathIndex = g.math.clamp( startingPoint, 1, 4 )
		currentPathTimes = horizontalBowTiePathTimes
	elseif( movementPath == "VERTICAL BOWTIE" ) then
		normalize( verticalBowTiePath )
		currentPath = verticalBowTiePath
		currentPathIndex = g.math.clamp( startingPoint, 1, 4 )
		currentPathTimes = verticalBowTiePathTimes
	elseif( movementPath == "STAR PATH" ) then
		normalize( starPath )
		currentPath = starPath
		currentPathIndex = g.math.clamp( startingPoint, 1, 5 )
		currentPathTimes = starPathTimes
	end
	
	local i = 1
	while( currentPathTimes[i] ) do
		currentPathTimes[i] = currentPathTimes[i] * scaleOfPath
		i = i + 1
	end
	i = 1
	while( currentPath[i] ) do
		currentPath[i].x = currentPath[i].x * pathDir
		currentPath[i].y = currentPath[i].y * pathDir
		i = i + 1
	end
	
	
end

function init()
	--JS: Set actor properties from UI values
	maxSpeed.x  = g.math.clamp( g.math.abs( moveSpeed ), 0, 128 )
	maxSpeed.y  = -g.math.clamp( g.math.abs( jumpSpeed ), 0, 128 )
	friction    = g.math.clamp( friction, 0, 1 )
	maxHealth   = g.math.clamp( health, 1, 65535 )
	healthRegen = g.math.clamp( healthRegen, 0, 65535 )
	maxArmor    = g.math.clamp( armor, 0, 65535 )
	armorRegen  = g.math.clamp( armorRegen, 0, 65535 )
	maxLives    = g.math.clamp( lives, 1, 65535 )
	maxStunTime = g.math.clamp( stunTime, 0, 65535 )
	damageToDeal = g.math.clamp( damageToDeal, 0, 128 )
	
	if( doesChasePlayer ) then
		chaseRadius = g.math.clamp( chaseRadius, 0, 500.0 )
	end
	
	setPathing()
	
	initialX = rigidBody:posX()
	initialY = rigidBody:posY()
	
	prevPos = vec2( initialX, initialY )
	
	--JS: Give this actor the correct starting properties
	SetActorProperties()
	damageReceived = 0
	
	
	--If this actor has no Combat component, set default combat to COLLISION, 1 damage per hit
	--TODO: replace single Combat component with 3 separates
	if ( this:isType( "Combat" ) == false ) then
		damageType = "COLLISION"
		damageToGive = 1
	end

	initialized = true
	isAlive     = true

	if ( debugComponent ) then
		debugPrint( this.id .. " : AI : init: Initialized" )
	end
end


function update( dt ) 
	if ( debugComponent ) or ( aiDrawCollision ) then
		debugDrawCollision( this, {255,255,0} )
	end
end


function fixedUpdate( dt )
	if ( initialized and (g.isPaused == false) and ( g.freezeTimeActive == false ) ) then
		if ( frozenDuringCollision == false ) then
			local vx = 0
			local vy = 0
		
			local playerX = g.player.rigidBody:posX()
			local playerY = g.player.rigidBody:posY()
			local aiX = rigidBody:posX()
			local aiY = rigidBody:posY()
			
			local distX = playerX - aiX
			local distY = playerY - aiY
			
			local chaseVector = vec2( 0, 0 )
			
			if( doesChasePlayer ) then
				local dist = sqrt( distX*distX + distY*distY )
				if( dist <= chaseRadius ) then
					isChasingPlayer = true
					updateAlways = true
				elseif( isChasingPlayer ) then
					isChasingPlayer = false
					g.audioManager.fadeAudioOut( chasingSoundEffect, 1 )
					isPlayingChaseSE = false
					updateAlways = false
				end
			end
			
			if( isChasingPlayer ) then
				chaseVector = vec2( distX, distY )
				normalize( {chaseVector, nil} )
				vx = chaseVector.x * moveSpeed
				vy = chaseVector.y * moveSpeed
				
				if( isPlayingChaseSE ~= true ) then
					g.audioManager.playAudio( chasingSoundEffect, true )
					isPlayingChaseSE = true
				end
				
				if( vx > 0 ) then
					iconAnim:scaleX( -1 )
				else
					iconAnim:scaleX( 1 )
				end
			else
				if( timeOnPath > currentPathTimes[currentPathIndex] ) then
					timeOnPath = 0
					if( currentPath[ currentPathIndex + pathDir ] ~= nil ) then
						currentPathIndex = currentPathIndex + pathDir
					else
						if( pathDir == -1 ) then
							currentPathIndex = g.table.getn( currentPath )
						else
							currentPathIndex = 1
						end
					end				
					

				else				
					newSpeed.x = currentPath[currentPathIndex].x * moveSpeed
					newSpeed.y = currentPath[currentPathIndex].y * moveSpeed
					
					timeOnPath = timeOnPath + dt + timeModifier
				end			

				vx = newSpeed.x
				vy = newSpeed.y
			end


			vx = vx + vx*modifier
			vy = vy + vy*modifier						
							
						
			newX = prevPos.x + vx * dt
			newY = prevPos.y + vy * dt
			prevPos = vec2( newX, newY )
			
			--rigidBody:setLinearVelocity( vx, vy )	
			rigidBody:setPos( newX, newY )
					
		end
		
		justDidDamage = false

		newState = "idle"
		if ( isChasingPlayer ) then
			newState = "chase"
		end

	elseif( g.freezeTimeActive or g.isPaused ) then
		if( isChasingPlayer ) then
			g.audioManager.stopAudio( chasingSoundEffect )
			isPlayingChaseSE = false
		end
	end
end

function drawPath()		

	local currentX = initialX
	local currentY = initialY
	local nextX = initialX + currentPath[startingPoint].x * currentPathTimes[startingPoint] * moveSpeed
	local nextY = initialY + currentPath[startingPoint].y * currentPathTimes[startingPoint] * moveSpeed
	drawLine( currentX, currentY, nextX, nextY , 0.0, 0.0, 0.0 )
	currentX = nextX
	currentY = nextY
	local i = startingPoint + pathDir
	if( i < 0 ) then
		i = g.table.getn( currentPath )
	end
	local color = 0.2
	local notDone = true
	while( notDone ) do
		if( i == startingPoint ) then
			notDone = false
		elseif( currentPath[i] ) then
			nextX = currentX + currentPath[i].x * currentPathTimes[i] * moveSpeed
			nextY = currentY + currentPath[i].y * currentPathTimes[i] * moveSpeed
			drawLine( currentX, currentY, nextX, nextY , 0.0, color, 0.0 )
			i = i + pathDir
			color = color * 2
			currentX = nextX
			currentY = nextY
		else
			if( pathDir == -1 ) then
				i = g.table.getn( currentPath )
			else
				i = 1
			end
		end
		
	end
end

function render( dt ) 
	if ( initialized ) then
		if ( debugComponent ) then
			if ( rigidBody:linearVelocityX() > 0 ) then
				drawLine( rigidBody:posX(), rigidBody:posY(), rigidBody:posX() + (iconW/pixelsPerUnit)/1.5, rigidBody:posY(), 0, 255, 0 )
			elseif ( rigidBody:linearVelocityX() < 0 ) then
				drawLine( rigidBody:posX(), rigidBody:posY(), rigidBody:posX() -	 (iconW/pixelsPerUnit)/1.5, rigidBody:posY(), 0, 255, 0 )
			end
			drawPath()
			if( isChasingPlayer ) then
				devFont:draw( 10, 200, "Chasing player" )
				devFont:draw( 10, 250, "Current State: " .. newState )				
			end
		end
	end
end


function beginContact( collidedWith )
	if ( initialized ) and ( isAlive ) then
		--Don't collide with hidden actors
		if ( collidedWith.iconVisible == false ) then
			if ( debugComponent ) then
				debugPrint( this.id .. " : PLAYER : beginContact : Actor " .. collidedWith.name .. "." .. collidedWith.id .. " is hidden. Skipping beginContact" )
			end
			return
		end
		
		if ( collidedWith:isType( "CC_script_player" ) ) and ( collidedWith.iconVisible ) and ( justDidDamage ~= true ) then
			g.player.currentHealth = g.player.currentHealth - damageToDeal
			g.audioManager.playAudio( collisionSoundEffect, false )			
			justDidDamage = true
		end

	end
end



function SetActorProperties( self )
	currentHealth  = maxHealth
	currentArmor   = maxArmor
	currentLives   = maxLives
end