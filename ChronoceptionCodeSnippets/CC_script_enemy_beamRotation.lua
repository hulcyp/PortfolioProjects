--#require CC_script_enemy_ai

priority = 1

radius = 0
modifier = 0
timeModifier =0

ui = {}

local maxAngle = g.pi / 2 - 0.00001

local rotationSpeed = g.pi / 4
local dir = 1

local center = vec2( 0, 0 )

local xPos = 0
local yPos = 0
local prevAngle = 0

--Debugging
local devFont = Font( "arial", 16 )
local debugComponent = false


function init()
	angle = -g.pi
	xPos = rigidBody:posX();
	yPos = rigidBody:posY();
	rigidBody:setFixedRotation( false )	
	
	radius = (iconH/pixelsPerUnit) / 2
	center = vec2( xPos, yPos - radius )
	rigidBody:setFixedRotation( false )
	
	
	updateAlways = true
end

function fixedUpdate( dt )
	if( g.isPaused == false and not g.freezeTimeActive) then
		if( angle >= maxAngle ) then
			dir = -1
		end
		if( angle <= -maxAngle ) then
			dir = 1
		end
		
		local rot = rotationSpeed + rotationSpeed*modifier 
		
		angle = angle + dir* rot*dt
		rigidBody:setAngularVelocity( dir*rot )

		local cosRot = radius*cos( angle )
		local sinRot = -radius*sin( angle )
		
		local x = sinRot + center.x
		local y = cosRot + center.y	
		
		rigidBody:setPos( x, y )
	else
		rigidBody:setAngularVelocity( 0 )
	end
	
end

function render()	
	if( debugComponent ) then
		devFont:draw( 10, 250, "Angle: " .. angle )
		devFont:draw( 10, 300, "Max angle: " .. maxAngle )
		devFont:draw( 10, 350, "Radius: " .. radius )
		devFont:draw( 10, 400, "Previous angle: " .. prevAngle )
		devFont:draw( 10, 450, "Dir: " .. dir )
		debugDrawCollision( this, {255,255,0} )
	end
end