#pragma once

#include "vector2.h"
#include "bitmapHandler.h"
#include "primitiveRenderer.h"

namespace game
{
	class UpdatableObject
	{
	public:
		virtual void Update()
		{
			std::cout << "update" << std::endl;
		}

		virtual ~UpdatableObject() {};
	};

	static std::vector<UpdatableObject*> UpdatableVector; // wektor wszystkich obiektow ktore sa updatowane z timer eventem
	

	class GameObject
	{
	public:
		GameObject() = default;
		virtual ~GameObject() {}
		PrimitiveRenderer& renderer = PrimitiveRenderer::getInstance();
	};

	class AnimatedObject 
	{
	public:
		AnimatedObject() = default;
		virtual ~AnimatedObject() {}

		virtual void Animate() = 0;
	};


	class BitmapObject : virtual public GameObject,virtual public UpdatableObject
	{
	public:
		BitmapObject() = default;

		virtual void Draw() = 0;
		void SetOrientation(float degrees);
		void SetScale(float scale);

		float GetOrientation();
		float GetScale();

		bool LoadFromFile(const char* filename);
		void Update();

	protected:
		BitmapHandler _handler;
		Vector2 _pos = { 0.0f, 0.0f };
		float _scale=1;
		float _orientation=0;
	};


	class GeometryObject : virtual public GameObject, public UpdatableObject
	{
	public:
		GeometryObject() = default;

		GeometryObject(Vector2 pos,std::vector<Point2D>& polygon)
		{
			_pos = pos;
			_polygon = polygon;

			for (Point2D& point : polygon) {
				_points.push_back(point + _pos);
			}

			UpdatableVector.push_back(this);

		}

		void Set_pos(Vector2 pos);
		void Set_points(std::vector<Point2D> points);
		void Set_polygon(std::vector<Point2D> polygon);
		void Set_thickness(float thickness);
		void Set_color(ALLEGRO_COLOR color);

		Vector2 Get_pos();
		std::vector<Point2D> Get_points();
		std::vector<Point2D> Get_polygon();
		float Get_thickness();
		ALLEGRO_COLOR Get_color();

		void Draw();
		void Update();

	protected:
		Vector2 _pos = { 0.0f, 0.0f };
		std::vector<Point2D> _points;
		std::vector<Point2D> _polygon;
		float _thickness = 1;
		ALLEGRO_COLOR _color = al_map_rgb(100, 100, 100);
	};


	class TransformableObject : public GeometryObject
	{
	public:
		TransformableObject() = default;

		TransformableObject(Vector2 pos, std::vector<Point2D>& polygon)
		{
			_pos = pos;
			_polygon = polygon;
			_scale = 1;

			for (Point2D& point : polygon) {
				_points.push_back(point + _pos);
			}

			UpdatableVector.push_back(this);
		}

		void Move(const Vector2&);
		void Rotate(float degrees);
		void Scale(float scaleNumber);
		void Update();
		float GetScale();
		float GetOrientation();

	protected:
		float _scale = 1;
		float _orientation = 0; // w k�tach nie w radianach
		//float _thickness = 1;
		//ALLEGRO_COLOR _color = al_map_rgb(100, 100, 100);
	};

	class SpriteObject : public BitmapObject, public AnimatedObject {
	public:
		//SpriteObject() : anim(nullptr), currentFrame(0), frameCount(0), frameDelay(5), maxFrame(0) {}
		SpriteObject()
		{
			UpdatableVector.push_back(this);
		}
		~SpriteObject();
		void Animate();
		void Draw();
		void SetSprite(const char* spriteFilename, short int frameIndex);
		void Move(const Vector2& vec);
		void Update();
	private:
		ALLEGRO_BITMAP** anim=nullptr;
		int currentFrame=0;
		int frameCount=0;
		int frameDelay=5;
		short int maxFrame=0;
	};

	class Player : public SpriteObject
	{
	public:
		//Player() = default;

		Player()
		{
			UpdatableVector.push_back(this);
		}

		void Draw();
		void Move(const Vector2& vec);
		void Update();
	};

	class Polygon : public TransformableObject
	{
	public:
		Polygon(Vector2 pos, std::vector<Point2D>& polygon) : TransformableObject(pos, polygon) {}
	};

	class Projectile :public UpdatableObject, virtual public GameObject
	{
	public:
		Projectile(Vector2 pos, float radius, ALLEGRO_COLOR color, Vector2 velocity)
		{
			UpdatableVector.push_back(this);

			_pos = pos;
			_radius = radius;
			_color = color;
			_velocity = velocity;

			std::cout << "projectile created" << std::endl;
		}

		void Update();
		void Draw();

		~Projectile() {}

	protected:
		Vector2 _pos = { 0.0f,0.0f };
		float _radius = 0;
		ALLEGRO_COLOR _color = al_map_rgb(255, 0, 0);
		Vector2 _velocity = { 0.0f,0.0f };
	};

}