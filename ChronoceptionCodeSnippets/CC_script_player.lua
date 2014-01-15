--#require CC_script_collision


--JS: File load and execution priority
priority = -950


--UI Variables
health        = 1

--Player animation state controllers
initialized = false
isAlive     = false
newState    = ""

--PH: animation state controls
isColliding = false

--Player properties
maxHealth      = 1
currentHealth  = 0
healthRegen    = 0
stunTime       = 8/14
deathDelay     = 0
currentDeathDelay = 0
drawCollision  	= false
maxSpeed  		= vec2( 0, 0 )
newSpeed		= vec2( 0, 0 )
friction  		= 0.9
regensElapsedTime	= 0.0

--Player movement speed
horizontalSpeed = 0
verticalSpeed = 0

--For conteracting gravity
oldPos = vec2( 0, 0 )

age = 30

healthReductionModifier = 1.0
		
levelToLoad = g.thisLevel
isFading = false
isEnding = false

spawnBG = false

buffer = 1.0
bufferSet = false

totalNumPickupsCollected = 0

--Debugging stuff
local debugComponent = false
local devFont = Font( "arial", 16 )
local isInvincible = false

ui = {
	horizontalSpeed  	= { order = 1,  type = "number",  label = "Horizontal Speed" },
	verticalSpeed    	= { order = 2,  type = "number",  label = "Vertical Speed" },
	friction       		= { order = 3,  type = "number",  label = "Floor Friction" },
	health         		= { order = 4,  type = "number",  label = "Max Health" },
	healthRegen    		= { order = 5,  type = "number",  label = "Health Regen/Sec" },
	healthReductionModifier = { order = 7, type = "number", label = "Factor to reduce health by" },
	deathDelay     		= { order = 8, type = "number",  label = "Respawn delay after death" },
	drawCollision  		= { order = 10, type = "bool", label = "Show collision?" },
}


function InitializePlayer()
	if ( g.player ~= this ) then
		g.player = this -- store player as global
	end
	
	--JS: Player Collision is hardcoded to Circle for now, 
	--JS:   until we can fix bugs related to box movement colliding with ghost vertices
	if ( rigidBody ~= nil ) then
		g.physics:remove( rigidBody )
	end
	rigidBody = g.physics:createCircle( x, y, RigidBody_DYNAMIC, g.math.max( iconW/pixelsPerUnit, iconH/pixelsPerUnit ), 1.0 )
	rigidBody:setCollisionCategory( CollisionCategory_Player )
	rigidBody:setCollisionMask( CollisionMask_Player )	

	--JS: Set actor properties from UI values
	maxSpeed.x  = g.math.clamp( g.math.abs( horizontalSpeed ), 0.1, 128 )
	maxSpeed.y  = -g.math.clamp( g.math.abs( verticalSpeed ), 0, 128 )
	friction    = g.math.clamp( friction, 0, 1 )
	maxHealth   = g.math.clamp( health, 1, 65535 )
	healthRegen = g.math.clamp( healthRegen, 0, 65535 )
	maxStunTime = g.math.clamp( stunTime, 0, 65535 )
	
	currentHealth = maxHealth
	levelToLoad = g.thisLevel
	

	currentDeathDelay = deathDelay
	oldPos = vec2( rigidBody:posX(), rigidBody:posY() )
		

	initialized = true
	isAlive     = true

	--Set current level in session variables
	--SetSessionVariable( "currentLevel", g.thisLevel )

	if ( debugComponent ) then
		debugPrint( this.id .. " : PLAYER : init: Initialized" )
	end

	--JFS: Ensure this actor is never "destroyed" during screen clipping, actor deletion etc.
	updateAlways = true
end


function init()
	InitializePlayer()
end


function update( dt ) 
	if ( debugComponent ) or ( drawCollision ) then
		debugDrawCollision( this, {255,255,0} )
	end
	
	if( not bufferSet ) then
		buffer = height()*.0016
	end

	if ( initialized and (g.isPaused == false) ) then
		
		if ( isAlive and (worldTimeValue >= 0 ) and (isColliding == false ) and currentHealth < maxHealth) then
			ApplyRegens( dt )
		end
		
		if( not isAlive ) then
			currentDeathDelay = currentDeathDelay - dt
		end	
	end
end


--JS: This is pretty messy, feels like it could be cleaned up a lot :/
function fixedUpdate( dt )
	if ( initialized ) then
	
		if ( not spawnBG ) then
			local star = spawn( x, y, g.starBackground, "BackgroundStars" )
			spawnBG = true
		end
		
		if( isInvincible ) then
			currentHealth = maxHealth
		end
		
				
		if( g.isPaused ) then
			rigidBody:setLinearVelocity( 0, 0 )	
			rigidBody:setPos( oldPos.x, oldPos.y )
			DisablePlayerInput()
		else
			
			if( not isAlive ) then
				if( not isFading ) then
					local thisEvent = new( g.eventStruct )
					thisEvent = g.EventSystem_LocateEventMapping( "evtCC_script_playerFadeScreen" )
					if( thisEvent ~= nil ) then
						thisEvent.sender = this.id
						thisEvent.destinationIDs = { this.id }
						EventClientComponent_PostEvent( thisEvent )
					end
					EventClientComponent_PostEvent( thisEvent )
					isFading = true
					g.audioManager.playAudio( "playerDeath", false )
					g.audioManager.fadeAudioOut( "playerDeath", deathDelay )
				end
				currentDeathDelay = currentDeathDelay - dt
			end
			
			if( ( not isAlive ) and ( currentDeathDelay <= 0 ) and not isEnding ) then
				local thisEvent = new( g.eventStruct )
				thisEvent = g.EventSystem_LocateEventMapping( "evtCC_script_playerGameOver" )
				if( thisEvent ~= nil ) then
					thisEvent.sender = this.id
					thisEvent.destinationIDs = { this.id }
					EventClientComponent_PostEvent( thisEvent )
				end
				iseEnding = true
				EventClientComponent_PostEvent( thisEvent )
			end
		
			if( currentHealth < 0 ) then
				isAlive = false
				DisablePlayerInput()
			end
			
			if ( isColliding ) then
				currentStunTime = currentStunTime + dt
				
				if ( currentStunTime >= maxStunTime ) then
					EnablePlayerInput()
					isColliding = false
				end
			end
			
			if( worldTimeValue < 0 ) then
				currentHealth = currentHealth + healthReductionModifier*( worldTimeValue ) * dt
			end
			
			age = 30
			if( currentHealth < 4*maxHealth / 5) then
				age = 40
			end
			if( currentHealth < 3*maxHealth / 5 ) then
				age = 50
			end
			if( currentHealth < 2*maxHealth / 5 ) then
				age = 60
			end
			if( currentHealth < maxHealth / 5 ) then
				age = 70				
			end	
			if( currentHealth < maxHealth / 10 ) then
				worldTimeValue = 0.0
			end

			vy = 0
		
			vx = maxSpeed.x
				
			if ( isAlive ) then
				if ( inputEnabled == false ) then
					EnablePlayerInput()
				end
				if( newSpeed.y ~= 0 ) then
					vy = newSpeed.y
					newSpeed.y = 0
				else
					vy = vy * ( 1 - friction )
					if ( vy > -0.1 ) and ( vy < 0.1 ) then
						vy = 0
						isMoving = false
					end
				end	
		
			end	

			newX = oldPos.x + vx*dt
			newY = oldPos.y + vy*dt
			
			
			if ( newY < Image_toGameY(0)+buffer ) then
				newY = Image_toGameY(0)+ buffer
			elseif ( newY > Image_toGameY(height())-buffer ) then
				newY = Image_toGameY(height())-buffer
			end

			
			oldPos = vec2( newX, newY )
			rigidBody:setLinearVelocity( vx, vy )	
			rigidBody:setPos( newX, newY )

			
			currentState = "stateIdleRight" .. age
			if ( isColliding ) then
				currentState = "stateHitRight" .. age
			end
			if( age == 40 ) then
				iconAnim:setTransparency( 0.85 )
			elseif( age == 50 ) then
				iconAnim:setTransparency( 0.75 )
			elseif( age == 60 ) then
				iconAnim:setTransparency( 0.65 )
			elseif( age == 70 ) then
				iconAnim:setTransparency( 0.55 )
			end
			if ( not isAlive ) then
				stateIdleRight = "stateDeathRight"
			end

		end
		
	end
end


function render( dt ) 
	if ( initialized ) and ( isAlive ) then
		if ( debugComponent ) then
			if ( rigidBody:linearVelocityX() > 0 ) then
				drawLine( rigidBody:posX(), rigidBody:posY(), rigidBody:posX() + (iconW/pixelsPerUnit)/1.5, rigidBody:posY(), 0, 255, 0 )
			elseif ( rigidBody:linearVelocityX() < 0 ) then
				drawLine( rigidBody:posX(), rigidBody:posY(), rigidBody:posX() -	 (iconW/pixelsPerUnit)/1.5, rigidBody:posY(), 0, 255, 0 )
			end
		end
	end
	drawLine( 0, 0, 0, 0, 0.0, 1.0, 0 );
	if( debugComponent ) then
		devFont:draw( 10, 40, "Pickups collected: " .. totalNumPickupsCollected )
		devFont:draw( 10, 80, "New State: " .. newState )
		devFont:draw( 10, 120, "Time value: " .. worldTimeValue )
		if( isColliding ) then
			devFont:draw( 10, 160, "COLLIDING" )
		end
		devFont:draw( 10, 200, "Death delay: " .. currentDeathDelay )
		devFont:draw( 10, 240, "Health: " .. currentHealth )
		devFont:draw( 10, 280, "Horizontal Speed: " .. rigidBody:linearVelocityX() )
	end
	
end


function beginContact( collidedWith ) 
	if ( initialized ) and ( isAlive ) then
		
		if( debugComponent ) then
			debugPrint( "I AM COLLIDING" )
		end
		--Don't collide with hidden actors
		if ( collidedWith.iconVisible == false ) then
			if ( debugComponent ) then
				debugPrint( this.id .. " : PLAYER : beginContact : Actor " .. collidedWith.name .. "." .. collidedWith.id .. " is hidden. Skipping beginContact" )
			end
			return
		end
		
		if( isInvincible == false ) then
			if ( collidedWith:isType( "CC_script_enemy_ai" ) ) and ( collidedWith.iconVisible ) and ( isColliding == false ) then
				isColliding = true
				currentStunTime = 0				
			end
		end		
	end
end


function ApplyRegens( dt )
	--MM: Only worry about elapsed time if something is regenerating
	if ( healthRegen > 0 )  then
		--MM: Add to elapsed time
		regensElapsedTime = regensElapsedTime + dt;
		
		--MM: Only regen every second
		if ( regensElapsedTime >= 1 ) then
			--MM: Reset elapsed time with out losing any time
			regensElapsedTime = regensElapsedTime - 1; 
				
			--MM: Regen Health
			if ( healthRegen > 0 ) then
				currentHealth = currentHealth + healthRegen;
				g.math.clamp( currentHealth, 0, maxHealth )
			end		
			
		end
	end
end



function StorePlayerScoreForLevel( self )
	--Make sure that this is not the tutorial player
	if( not this:isType( "CC_script_tutorial" ) ) then
		local total = g.session.totalNumPickups or 0
		total = total + g.player.totalNumPickupsCollected + g.player.currentNumOfPickups		
		g.session.totalNumPickups = ( g.session.totalNumPickups or 0 ) + total
		if( debugComponent ) then
			debugPrint( "PLAYERS : Pickups collected: " .. total )
			debugPrint( "PLAYERS : Storing player information" )
		end
	end	
end

function EnablePlayerInput()
	EnablePlayerInputXbox()
	EnablePlayerInputKeyboard()
end

function DisablePlayerInput()
	DisablePlayerInputXbox()
	DisablePlayerInputKeyboard()
end