/*
    Enki - a fast 2D robot simulator
    Copyright (C) 1999-2005 Stephane Magnenat <nct@ysagoon.com>
    Copyright (C) 2005 Laboratory of Intelligent Systems, EPFL, Lausanne
    See AUTHORS for details

    This program is free software; the authors of any publication 
    arising from research using this software are asked to add the 
    following reference:
    Enki - a fast 2D robot simulator part of the Teem framework
    http://teem.epfl.ch
    Stephane Magnenat <stephane.magnenat@a3.epfl.ch>,
    Markus Waibel <markus.waibel@epfl.ch>
    Laboratory of Intelligent Systems, EPFL, Lausanne.

    You can redistribute this program and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __SBOT_H
#define __SBOT_H

#include <enki/PhysicalEngine.h>
#include <enki/interactions/Microphone.h>
#include <enki/robots/s-bot/SbotCam.h>
#include <enki/robots/s-bot/ActiveSoundObject.h>
#include <enki/robots/s-bot/SbotObject.h>

/*!	\file Sbot.h
	\brief Header of the Sbot robot
*/

namespace Enki
{
	//! Interaction sound between all Sbots.
	/*! The Sbots are supposed to emit sound at a sufficiently high intensity such as everyone hears it.
		\ingroup interaction
	*/
	class SbotGlobalSound : public GlobalInteraction
	{
	protected:
		//! The world frequencies state, mask of all frequencies
		static unsigned worldFrequenciesState;
		
	public:
		//! The frequencies state of this robot, mask of all frequencies
		unsigned frequenciesState;
		
	public:
		//! Constructor
		SbotGlobalSound::SbotGlobalSound (Robot *me) { this->owner = me; }
		//! Initialisation, set world frequencies to zero. Called one time for each robot, which could be optimised.
		virtual void init() { worldFrequenciesState = 0; }
		//! Emit our frequencies to the world
		virtual void step(double dt, World *w) { worldFrequenciesState |= frequenciesState; }
		//! Return state of the frequencies in the world
		static unsigned getWorldFrequenciesState(void);
	};


	//! Specific microphone for S-bots
	/*! This microphone checks whether there are sounds coming from
	  sound-emitting objects, and also other s-bots
	class SbotMicrophone : public Microphone	{
	public:
		//! Constructor
		//! e.g.: Microphone(this, An::Vector(0.5, 0.5), 5, micStepModel, 20);
		//! meaning: the mic is (0.5, 0.5) away from robot center, can hear sounds up to
		//! 5 units away, uses a step model to detect sounds and can distinguish 20 frequencies
		SbotMicrophone(Robot *owner, An::Vector micRelPos, double range, 
					   MicrophoneResponseModel micModel, unsigned channels) :
			Microphone(owner, micRelPos, range, micModel, channels) {}
		//! Check for local interactions with other physical objects
		void objectStep(double dt, PhysicalObject *po, World *w);
	};
	*/

	//! Specific microphone for S-bots
	/*! This microphone checks whether there are sounds coming from
	  sound-emitting objects, and also other s-bots
	*/
	class SbotMicrophone : public FourWayMic {
	public:
		//! Constructor
		//! e.g.: FourWayMic(this, 0.5, 5, micStepModel, 20);
		//! meaning: the 4 mics are 0.5 away from robot center, can
		//hear sounds up to ! 5 units away, uses a step model to
		//detect sounds and can distinguish 20 frequencies
		SbotMicrophone(Robot *owner, double micDist, double range, 
					   MicrophoneResponseModel micModel, unsigned channels) :
			FourWayMic(owner, micDist, range, micModel, channels) {}
		//! Check for local interactions with other physical objects
		void objectStep(double dt, PhysicalObject *po, World *w);
	};

	//! A very simplified model of the Sbot mobile robot.
	/*! Only implement a subset of the camera
		\ingroup robot
	*/
	class Sbot : public Robot
	{
	public:
		//! The omnidirectional linear camera
		SbotCam camera;
		//! the sound interaction, based on global frequencies
		SbotGlobalSound globalSound;

	public:
		//! Speed of the robot
		double leftSpeed, rightSpeed;

	public:
		//! Constructor
		Sbot();
		//! Destructor
		~Sbot() {}
		//! Set the real speed of the robot given leftSpeed and rightSpeed. Add noise
		virtual void step(double dt);
	};


	// TODO : handle this nicely
	
	//! An "improved" Sbot that can interact with SbotActiveObject.
	/*! It is a hack for my experiments, should be removed one day.
		\ingroup robot
	*/
	class FeedableSbot : public Sbot
	{
	public:
		//! The actual energy of the Sbot
		double energy;
		//! The actual energy difference
		double dEnergy;
		//! The previous energy difference
		double lastDEnergy;

		//! Constructor
		FeedableSbot() { energy=0; dEnergy=0; lastDEnergy=0; }
		//! Call Sbot::step and compute the new energy
		virtual void step(double dt) ;
	};


	//! This Sbot version has sound capabilities inherited from
	//! ActiveSoundObject, as well as the feeding and usual capabilities
	//! of an Sbot.
	/*\ingroup robot*/

	class SoundSbot : public FeedableSbot
	{
	public:
		//! 4 microphones
		/*
		SbotMicrophone mic0;
		SbotMicrophone mic1;
		SbotMicrophone mic2;
		SbotMicrophone mic3;
		*/
		SbotMicrophone mic;
		//! 1 speaker
		ActiveSoundSource speaker;
		virtual void step(double dt);

	public:
		//! Constructor, initialises microphones and speaker
		SoundSbot();
	};
}
#endif
