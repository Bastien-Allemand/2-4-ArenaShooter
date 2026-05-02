#ifndef ENGINE_TWEENS_HPP_INCLUDED
#define ENGINE_TWEENS_HPP_INCLUDED

#include <Containers/Vector.hpp>
#include "define.h"
#include "Function.hpp"
#include "Maths/MathsFunctions.hpp"

namespace gce {


	template <typename T> class Tween;
	template <> class Tween<void>
	{
	protected:
		virtual ~Tween() = default;
		virtual void Update() = 0;
		friend class TweenSystem;
	};

	template <typename T>
	class Tween : public Tween<void>
	{
	public:
		void StartDuration(float32 duration, T& result, bool reverse = false);
		template <typename C> void StartDuration(float32 duration, Function<void(C::*)(T const&)> const& resultFunction, bool reverse = false);

		T start, end;
		float32(*pFunction)(float32) = nullptr;

	private:
		Tween() = default;
		~Tween() override = default;

		explicit Tween(T const& start, T const& end, float32(* const pFunction)(float32)) : start(start), end(end), pFunction(pFunction) {}

		Tween(Tween const&) = default;
		Tween(Tween&&) = default;
		Tween& operator=(Tween const&) = default;
		Tween& operator=(Tween&&) = default;

		void Update() override;

		struct ResultInstance { float32 duration, elapsed; T* pResult; bool reverse; };
		struct FunctionInstance { float32 duration, elapsed; Function<void(* const)(T const&)> const* pResultFunction; bool reverse; };

		Vector<ResultInstance*> m_resultInstances;
		Vector<FunctionInstance*> m_functionInstances;

		friend class TweenSystem;
	};


	class TweenSystem
	{
	public:
		template <typename T> static Tween<T>& Create();
		template <typename T> static Tween<T>& Create(T const& start, T const& end, float32(*pFunction)(float32));
		template <typename T> static void Delete(Tween<T>& tween);

		static void Update(float32 deltaTime);
		static float32 DeltaTime() { return s_deltaTime; }

	private:
		inline static Vector<Tween<void>*> s_tweens;
		inline static float32 s_deltaTime;
	};


	template <typename T>
	void Tween<T>::StartDuration(float32 const duration, T& result, bool const reverse)
	{
		m_resultInstances.PushBack(new ResultInstance{ duration, 0.0f, &result, reverse });
	}

	template <typename T> template <typename C>
	void Tween<T>::StartDuration(float32 const duration, Function<void(C::*)(T const&)> const& resultFunction, bool const reverse)
	{
		m_functionInstances.PushBack(new FunctionInstance{ duration, 0.0f, new Function<void(C::*)(T const&)>(resultFunction), reverse });
	}


	template <typename T>
	void Tween<T>::Update()
	{
		float32 const deltaTime = TweenSystem::DeltaTime();

		for (auto it = m_resultInstances.begin(); it != m_resultInstances.end(); )
		{
			auto& [duration, elapsed, pResult, reverse] = **it;
			elapsed += deltaTime;

			float32 const progress = reverse ? 1.0f - elapsed / duration : elapsed / duration;
			*pResult = start + (end - start) * pFunction(progress <= 1.0f ? progress : 1.0f);

			if (elapsed >= duration)
			{
				delete* it;
				it = m_resultInstances.Erase(it);
				continue;
			}
			++it;
		}

		for (auto it = m_functionInstances.begin(); it != m_functionInstances.end(); )
		{
			auto& [duration, elapsed, pResultFunction, reverse] = **it;
			elapsed += deltaTime;

			float32 const progress = reverse ? 1.0f - elapsed / duration : elapsed / duration;
			(*pResultFunction)(start + (end - start) * pFunction(progress <= 1.0f ? progress : 1.0f));

			if (elapsed >= duration)
			{
				delete (*it)->pResultFunction;
				delete* it;
				it = m_functionInstances.Erase(it);
				continue;
			}
			++it;
		}
	}


	template <typename T>
	Tween<T>& TweenSystem::Create()
	{
		Tween<T>& tween = *new Tween<T>();
		s_tweens.PushBack(&tween);
		return tween;
	}

	template <typename T>
	Tween<T>& TweenSystem::Create(T const& start, T const& end, float32(* const pFunction)(float32))
	{
		Tween<T>& tween = *new Tween<T>(start, end, pFunction);
		s_tweens.PushBack(&tween);
		return tween;
	}

	template <typename T>
	void TweenSystem::Delete(Tween<T>& tween)
	{
		for (auto it = s_tweens.begin(); it != s_tweens.end(); ++it)
		{
			if (*it != &tween) continue;
			s_tweens.Erase(it);
			delete& tween;
			return;
		}
	}

	inline void TweenSystem::Update(float32 const deltaTime)
	{
		s_deltaTime = deltaTime;
		for (Tween<void>* pTween : s_tweens)
			pTween->Update();
	}


	struct Tweens
	{
	private:
		// Back
		static constexpr float32 BA_C = 1.70158f;
		static constexpr float32 BA_C2 = BA_C * 1.525f;

		// Elastic
		static constexpr float32 EL_C = 2.0f * PI / 3.0f;
		static constexpr float32 EL_C2 = 2.0f * PI / 4.5f;

		// Bounce
		static constexpr float32 BO_C = 7.5625f;
		static constexpr float32 BO_C2 = 2.75f;

		// Complex Function
		static constexpr float32 PULSE_FALLING_DURATION = 1.0f;
		static constexpr float32 PULSE_AMPLITUDE = 0.05f;

	public:
		// Linear
		static float32 Linear(float32 const x) { return x; }

		// Quadratic
		static float32 EaseIn(float32 const x) { return x * x; }
		static float32 EaseOut(float32 const x) { return 1.0f - (1.0f - x) * (1.0f - x); }
		static float32 EaseInOut(float32 const x) { return x < 0.5f ? 2.0f * x * x : (4.0f - 2.0f * x) * x - 1.0f; }

		// Sine
		static float32 EaseInSine(float32 const x) { return 1.0f - Cos(x * PI / 2.0f); }
		static float32 EaseOutSine(float32 const x) { return Sin(x * PI / 2.0f); }
		static float32 EaseInOutSine(float32 const x) { return (1.0f - Cos(PI * x)) / 2.0f; }

		// Cubic
		static float32 EaseInCubic(float32 const x) { return x * x * x; }
		static float32 EaseOutCubic(float32 const x) { return 1.0f - Pow(1.0f - x, 3); }
		static float32 EaseInOutCubic(float32 const x) { return x < 0.5f ? 4.0f * x * x * x : 1.0f - Pow(2.0f - 2.0f * x, 3) / 2.0f; }

		// Quart
		static float32 EaseInQuart(float32 const x) { return x * x * x * x; }
		static float32 EaseOutQuart(float32 const x) { return 1.0f - Pow(1.0f - x, 4); }
		static float32 EaseInOutQuart(float32 const x) { return x < 0.5f ? 8.0f * x * x * x * x : 1 - Pow(2.0f - 2.0f * x, 4) / 2.0f; }

		// Quint
		static float32 EaseInQuint(float32 const x) { return x * x * x * x * x; }
		static float32 EaseOutQuint(float32 const x) { return 1.0f - Pow(1.0f - x, 5); }
		static float32 EaseInOutQuint(float32 const x) { return x < 0.5f ? 16.0f * x * x * x * x * x : 1.0f - Pow(2.0f - 2.0f * x, 5) / 2.0f; }

		// Circular
		static float32 EaseInCirc(float32 const x) { return 1.0f - Sqrt(1.0f - x * x); }
		static float32 EaseOutCirc(float32 const x) { return Sqrt(1.0f - Pow(x - 1.0f, 2)); }
		// static float32 EaseInOutCirc( float32 const x ) { return ( x < 0.5f ? 1.0f - Sqrt( 1.0f - 4.0f * x * x) : Sqrt( -( 2.0f * x - 3.0f ) * ( 2.0f * x - 1.0f ) ) + 1.0f ) / 2.0f; }
		static float32 EaseInOutCirc(float32 const x) { return (x < 0.5f ? 1.0f - Sqrt(1.0f - 4.0f * x * x) : Sqrt(-4.0f * x * x + 8.0f * x - 3.0f) + 1.0f) / 2.0f; }

		// Back
		static float32 EaseInBack(float32 const x) { return x * x * (BA_C * (x - 1.0f) + x); }
		static float32 EaseOutBack(float32 const x) { return 1.0f + (x - 1.0f) * (x - 1.0f) * (BA_C * x + x - 1.0f); }
		// static float32 EaseInOutBack( float32 const x ) { return x < 0.5f ? Pow(2.0f * x, 2) * ((BACK_CONSTANT2 + 1.0f) * 2.0f * x - BACK_CONSTANT2) / 2.0f : (Pow(2.0f * x - 2.0f, 2) * ((BACK_CONSTANT2 + 1.0f) * (x * 2.0f - 2.0f) + BACK_CONSTANT2) + 2.0f) / 2.0f; }
		static float32 EaseInOutBack(float32 const x) { return (x < 0.5f ? Pow(2.0f * x, 2) * (BA_C2 * (2.0f * x - 1.0f) + 2.0f * x) : Pow(2.0f * x - 2.0f, 2) * (BA_C2 * (x * 2.0f - 1.0f) + x * 2.0f - 2.0f) + 2.0f) / 2.0f; }

		// Elastic
		static float32 EaseInElastic(float32 x);
		static float32 EaseOutElastic(float32 x);
		static float32 EaseInOutElastic(float32 x);

		// Bounce
		static float32 EaseInBounce(float32 const x) { return 1.0f - EaseOutBounce(1.0f - x); }
		static float32 EaseOutBounce(float32 x);
		static float32 EaseInOutBounce(float32 const x) { return (x < 0.5f ? EaseInBounce(2.0f * x) : 1.0f + EaseOutBounce(2.0f * x - 1.0f)) * 0.5f; }

		// Complex Function
		static float32 Pulse(float32 const x) { return x < PULSE_AMPLITUDE ? EaseInOutQuint(x / PULSE_AMPLITUDE) : EaseOut((1.0f - x) / PULSE_FALLING_DURATION); }
	};


	// Elastic

	inline float32 Tweens::EaseInElastic(float32 const x)
	{
		if (x == 0.0f || x == 1.0f) return x;
		return -Pow(2.0f, static_cast<uint32>(10.0f * x - 10.0f)) * Sin((10.0f * x - 10.75f) * EL_C);
	}

	inline float32 Tweens::EaseOutElastic(float32 const x)
	{
		if (x == 0.0f || x == 1.0f) return x;
		return Pow(2.0f, static_cast<uint32>(-10.0f * x)) * Sin((10.0f * x - 0.75f) * EL_C) + 1.0f;
	}

	inline float32 Tweens::EaseInOutElastic(float32 const x)
	{
		if (x == 0.0f || x == 1.0f) return x;
		return x < 0.5f ? -(Pow(2.0f, static_cast<uint32>(20.0f * x - 10.0f)) * Sin((20.0f * x - 11.125f) * EL_C2)) / 2.0f : Pow(2.0f, static_cast<uint32>(10.0f - 20.0f * x)) * Sin((20.0f * x - 11.125f) * EL_C2) / 2.0f + 1.0f;
	}

	// Bounce

	inline float32 Tweens::EaseOutBounce(float32 x)
	{
		if (x < 1.0f / BO_C2) return BO_C * x * x;
		if (x < 2.0f / BO_C2) return BO_C * (x -= 1.5f / BO_C2) * x + 0.75f;
		if (x < 2.5f / BO_C2) return BO_C * (x -= 2.25f / BO_C2) * x + 0.9375f;
		return BO_C * (x -= 2.625f / BO_C2) * x + 0.984375f;
	}


}



// class Tween 
// {
// private:
// 	using EasingFunction = float32(*)(float32);
//
//     // Update callbacks for different value types
//     using UpdateFloatFunction = void (*)(float32 value, void* pUser);
//     using UpdateVector3Function = void (*)(Vector3f32 const& value, void* pUser);
//
// 	// Update functions for GameObject transforms
// 	// static void UpdatePosition(Vector3f32 const& value, void* pUser) 
// 	// {
// 	// 	GameObject* obj = static_cast<GameObject*>(pUser);
// 	// 	obj->transform.SetLocalPosition(value);
// 	// }
//
// 	// static void UpdateScale(Vector3f32 const& value, void* pUser)
// 	// {
// 	// 	GameObject* obj = static_cast<GameObject*>(pUser);
// 	// 	obj->transform.SetLocalScale(value);
// 	// }
//
// 	// static void UpdateRotation(Vector3f32 const& value, void* pUser)
// 	// {
// 	// 	GameObject* obj = static_cast<GameObject*>(pUser);
// 	// 	obj->transform.SetLocalRotation(Quaternion::RotationEuler(value.x, value.y, value.z));
// 	// }
//
// 	/// Base TweenInstance class
//     class TweenInstance 
// 	{
//
//     public:
//         virtual ~TweenInstance() = default;
//         virtual void Update() = 0;
//
// 		bool IsRunning() const { return m_running; }
// 		bool IsPaused() const { return m_paused; }
// 		void Reset() { m_running = true; m_paused = true; m_elapsed = 0; }
// 		void Pause() { m_paused = true; }
// 		void Play() { m_paused = false; }
// 		void Stop() { m_running = false; }
// 		bool IsLooping() const { return m_loop; }
// 		void Loop(bool loop = true) { m_loop = loop; }
//
// 	protected:
// 		float32 m_duration;
// 		float32 m_elapsed;
// 		bool m_running;
// 		bool m_paused;
// 		EasingFunction m_easing;
// 		void* m_pUserPtr;
// 		bool m_loop = false;
//     };
//
// public:
//
// 	// Start a float tween with a pointer to a variable
// 	// @param Example : 0.0f, 5.0f, 4.0f, Easing::EaseInOutQuint, var, pGameObjet
//     static TweenInstance* Start(float32 start, float32 end, float32 duration, EasingFunction easing, float32& updateVariable, void* pUser)
// 	{
//
// 		FloatTweenInstance* tween = new FloatTweenInstance(start, end, duration, easing, updateVariable, pUser);
// 		s_activeTweens.push_back(tween);
// 		return tween;
//     }
//
// 	// Start a float tween with a lambda function
// 	// @param Example : 0.0f, 5.0f, 4.0f, Easing::EaseInOutQuint, [](float32 value, void* user) { /* Update logic */ }, pGameObjet
// 	static TweenInstance* Start(float32 start, float32 end, float32 duration, EasingFunction easing, UpdateFloatFunction updateFuction, void* pUser)
// 	{
//
// 		CustomTweenInstance* tween = new CustomTweenInstance(start, end, duration, easing, updateFuction, pUser);
// 		s_activeTweens.push_back(tween);
// 		return tween;
// 	}
//
//
// 	// Start a Vector3 tween with a GameObject transform
// 	// @param Example : {0, 0, 0}, {5, 0, 0}, 3.0f, Easing::EaseOutQuint, "position", pGameObjet
//     static TweenInstance* Start(Vector3f32 start, Vector3f32 end, float32 duration, EasingFunction easing, String const& transform, void* pUser)
// 	{
// 		UpdateVector3Function update = nullptr;
// 		// if (transform == "Position")
// 		// 	update = UpdatePosition;
// 		// else if (transform == "Scale")
// 		// 	update = UpdateScale;
// 		// else if (transform == "Rotation")
// 		// 	update = UpdateRotation;
// 		// else 
// 		// 	throw std::invalid_argument("Unknown transform type");
//
// 		Vector3TweenInstance* tween = new Vector3TweenInstance(start, end, duration, easing, update, pUser);
// 		s_activeTweens.push_back(tween);
// 		return tween;
//     }
//
//     // Update all active tweens
//     static void UpdateAll() 
// 	{
//         if (s_globalPaused) 
// 			return;
//
//         for (auto& tween : s_activeTweens)
//             tween->Update();
//     }
//
//     // Pause/Play all tweens
//     static void PauseAll()
// 	{
// 		s_globalPaused = true;
//         for (auto& tween : s_activeTweens)
//             tween->Pause();
//     }
//
//     static void PlayAll() 
// 	{
// 		s_globalPaused = false;
//         for (auto& tween : s_activeTweens)
//             tween->Play();
//     }
//
//     // Stop and clear all tweens
//     static void StopAll() 
// 	{
// 		for (auto& tween : s_activeTweens)
// 			tween->Stop();
// 		s_activeTweens.clear();
// 		s_globalPaused = false;
//     }
//
//     // Check if there are any running tweens
//     static bool HasRunningTweens() { return !s_activeTweens.empty(); }
//
//     // Get count of active tweens
// 	static size_t GetActiveTweenCount() { return s_activeTweens.size(); }
//
// private:
//
// 	/// Float tween instance
//     class FloatTweenInstance : public TweenInstance 
// 	{
//     public:
// 		FloatTweenInstance(float32 start, float32 end, float32 duration, EasingFunction easing, float32& updateVariable, void* pUser)
// 		: m_startValue(start), m_endValue(end), m_updateVariable(updateVariable)
// 		{
// 			m_duration = duration;
// 			m_elapsed = 0;
// 			m_running = true;
// 			m_paused = false;
// 			m_easing = easing;
// 			m_pUserPtr = pUser;
// 			m_loop = false;
// 		};
//
//         void Update() override 
// 		{
//             if (!m_running || m_paused) 
// 				return;
//
// 			// m_elapsed += GameManager::DeltaTime();
// 			m_elapsed += 0.016f; // TODO REMOVE
// 			
// 			float32 value = m_startValue + (m_endValue - m_startValue) * m_easing(1.0f);
//             if (m_elapsed >= m_duration) 
// 			{
// 				m_updateVariable = value;
//                 m_running = false;
//                 return;
//             }
//
//             float32 t = m_elapsed / m_duration;
//             float32 easedT = m_easing(t);
//             value = m_startValue + (m_endValue - m_startValue) * easedT;
// 			m_updateVariable = value;
//         }
//
//     private:
// 		float32 m_startValue;
// 		float32 m_endValue;
// 		float32& m_updateVariable;
//     };
//
// 	/// Vector3 tween instance
//     class Vector3TweenInstance : public TweenInstance 
// 	{
//     public:
// 		Vector3TweenInstance(Vector3f32 start, Vector3f32 end, float32 duration, EasingFunction easing, UpdateVector3Function Update, void* pUser)
// 		: m_startValue(start), m_endValue(end), m_updateFonction(Update)
// 		{
// 			m_duration = duration;
// 			m_elapsed = 0;
// 			m_running = true;
// 			m_paused = false;
// 			m_easing = easing;
// 			m_pUserPtr = pUser;
// 			m_loop = false;
// 		}
//
//         void Update() override 
// 		{
//             if (!m_running || m_paused)
// 				return;
//
// 			// m_elapsed += GameManager::DeltaTime();
// 			m_elapsed += 0.016f; // TODO REMOVE
//             
// 			Vector3f32 value = m_startValue + (m_endValue - m_startValue) * m_easing(1.0f);
//
// 			if (m_loop && m_elapsed >= m_duration)
// 				m_elapsed = 0;
// 			else if (m_elapsed >= m_duration && !m_loop) 
// 			{
// 				m_updateFonction(value, m_pUserPtr);
// 				m_running = false;
//                 return;
// 			}
//
//             float32 t = m_elapsed / m_duration;
//             float32 easedT = m_easing(t);
//             value = m_startValue + (m_endValue - m_startValue) * easedT;
// 			m_updateFonction(value, m_pUserPtr);
//         }
//
//     private:
// 		Vector3f32 m_startValue;
// 		Vector3f32 m_endValue;
//         UpdateVector3Function m_updateFonction;
//     };
//
// 	/// Custom float tween instance that allows for custom update functions
// 	class CustomTweenInstance : public TweenInstance 
// 	{
// 	public:
// 		CustomTweenInstance(float32 start, float32 end, float32 duration, EasingFunction easing, UpdateFloatFunction Update, void* pUser)
// 		: m_startValue(start), m_endValue(end), m_updateFonction(Update)
// 		{
// 			m_duration = duration; 
// 			m_elapsed = 0;
// 			m_running = true; 
// 			m_paused = false;
// 			m_easing = easing;
// 			m_pUserPtr = pUser;
// 			m_loop = false;
// 		}
//
// 		void Update() override 
// 		{
// 			if (!m_running || m_paused)
// 				return;
//
// 			// m_elapsed += GameManager::DeltaTime();
// 			m_elapsed += 0.016f; // TODO REMOVE
// 			
// 			float32 value = m_startValue + (m_endValue - m_startValue) * m_easing(1.0f);
// 			if (m_elapsed >= m_duration)
// 			{
// 				m_updateFonction(value, m_pUserPtr);
// 				m_running = false;
// 				return;
// 			}
//
// 			float32 t = m_elapsed / m_duration;
// 			float32 easedT = m_easing(t);
// 			value = m_startValue + (m_endValue - m_startValue) * easedT;
// 			m_updateFonction(value, m_pUserPtr);
// 		}
//
// 	private:
// 		float32 m_startValue;
// 		float32 m_endValue;
// 		UpdateFloatFunction m_updateFonction;
// 	};
//
//     // Static list of active tweens
//     inline static Vector<TweenInstance*> s_activeTweens;
//     inline static bool s_globalPaused = false;
//
// 	friend class Sequence;
// };
//
// // Sequence class to manage a sequence of tweens
// class Sequence 
// {
// public:
// 	Sequence() 
// 	{
// 		s_activeSequences.push_back(new SequenceInstance());
// 		m_currentSequence = s_activeSequences.back();
// 	}
//
// 	void AddTween(Tween::TweenInstance* tween, int _IndexAnim) { m_currentSequence->AddTween(tween, _IndexAnim); }
//
// 	static void UpdateAll()
// 	{
// 		for (auto* seq : s_activeSequences)
// 			seq->Update();
// 	}
//
// 	void Loop(bool loop = true)
// 	{
// 		if (m_currentSequence)
// 			m_currentSequence->Loop(loop);
// 	}
//
// private:
// 	class SequenceInstance
// 	{
// 	public:
// 		SequenceInstance() = default;
//
// 		void AddTween(Tween::TweenInstance* tween, uint64 _IndexAnim)
// 		{
// 			if (!m_tweens.empty() && _IndexAnim != m_currentTweens)
// 				tween->Pause();
// 			if (_IndexAnim >= m_tweens.size()) 
// 				m_tweens.resize(_IndexAnim + 1);
// 			m_tweens[_IndexAnim].push_back(tween);
// 		}
// 		
// 		// Allow the sequence to loop through all its tweens idefinitely
// 		void Loop(bool loop = true) { m_loop = loop; }
//
// 		void Update()
// 		{
// 			if (m_tweens.empty())
// 				return;
// 			for (auto& tween : m_tweens[m_currentTweens])
// 			{
// 				if (tween->IsRunning())
// 					return; // If not all tweens in the list are finished, we return
// 			}
//
// 			m_currentTweens++; // Increment the current tween index
//
// 			if (m_currentTweens >= m_tweens.size()) // If we have reached the end of the list
// 			{
// 				m_currentTweens = 0; // Reset to the first tween
//
// 				if (!m_loop) // If we are not looping, we reset the sequence
// 				{
// 					m_tweens.clear();
// 					return;
// 				}
//
// 				for (auto& list : m_tweens) // Reset all tweens
// 				{
// 					for (auto& tween : list) // Iterate through each tween in the list
// 						tween->Reset(); // Reset the tween
// 				}
// 			}
//
// 			for (auto& tween : m_tweens[m_currentTweens]) // Iterate through each tween in the new current list and play them
// 				tween->Play();
// 		}
//
// 	private:
// 		bool m_loop = false;
// 		uint32 m_currentTweens = 0;
// 		Vector<Vector<Tween::TweenInstance*>> m_tweens;
// 	};
//
// 	SequenceInstance* m_currentSequence = nullptr;
// 	inline static Vector<SequenceInstance*> s_activeSequences;
// 	friend class Tween;
// };
//
//
//
// void UpdateTweeningSystem()
// {
// 	Sequence::UpdateAll();
// 	Tween::UpdateAll();
// }


#endif