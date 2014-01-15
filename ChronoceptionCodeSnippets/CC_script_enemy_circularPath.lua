--#require CC_script_enemy_ai

priority = 2

radius = 16
modifier = 0
timeModifier =0

ui = {
		radius 	= { order = 1, type = "number", label = "Radius of Rotation" },
	}
	
local maxAngle = g.pi / 2 - 0.00000004
local devFont = Font( "arial", 16 )
local rotationSpeed = g.pi / 4
local dir = 1
local center = vec2( 0, 0 )

local xPos = 0
local yPos = 0

local debugComponent = false

function init()
	angle = 0
	xPos = rigidBody:posX();
	yPos = rigidBody:posY();
	rigidBody:setFixedRotation( false )
	
	center = vec2( xPos, yPos - radius )
	
	updateAlways = true
	
end

function fixedUpdate( dt )
	
	if( g.isPaused == false and not g.freezeTimeActive ) then
		if( angle >= maxAngle ) then
			dir = -1
		end
		if( angle <= -maxAngle ) then
			dir = 1
		end
		
		local rot = rotationSpeed + rotationSpeed*modifier
		angle = angle + dir* rot*dt
		
		local cosRot = radius*cos( angle )
		local sinRot = radius*sin( angle )
		
		local x = sinRot + center.x
		local y = cosRot + center.y	
		rigidBody:setAngularVelocity  ( dir*rot )
		rigidBody:setPos( x, y )
	end
end

function render()
	if( debugComponent ) then
		debugDrawCollision( this, {255,255,0} )
	end
end