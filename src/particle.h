
class Particle
{
private:
	int x_;
	int y_;
	float charge_;

public:
	Particle(float charge)
	{
		charge_ = charge;
	}
	float GetCharge();
};
