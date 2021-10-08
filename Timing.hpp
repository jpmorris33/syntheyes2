class Timing {
	public:
		virtual void set(int ms);
		virtual bool elapsed();
		virtual void wait_microseconds(int us);
};
