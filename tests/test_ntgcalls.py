import unittest
from ntgcalls import NTgCalls


class TestNTgCalls(unittest.IsolatedAsyncioTestCase):
    async def test_ping(self):
        result = NTgCalls().ping()
        self.assertEqual(result, "pong")
        self.assertIsNotNone(result)

    async def test_shutdown_is_idempotent(self):
        session = NTgCalls()
        session.shutdown()
        session.shutdown()


if __name__ == "__main__":
    unittest.main()
