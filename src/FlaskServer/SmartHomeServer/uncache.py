# This code snippet will append all static files
# with ?upd_time=XXXX where XXXX is the timestamp of the file.
# This will help so that when the files in the server are changed, the
# browser would not automatically load from local cache, but instead
# redownload it from the server.

from os import stat, path
from flask.helpers import url_for

def reg_static_uncache(app):

    @app.context_processor
    def override_url_for():

        def dated_url_for(endpoint, **values):

            # Will change the url variable to the file's timestamp
            # so that browser caching system will update.
            if endpoint.startswith("static"):
                filename = values.get('filename')
                if filename:
                    values['upd_time'] = int(stat(path.join(app.root_path, endpoint, filename)).st_mtime)

            return url_for(endpoint, **values)

        return dict(url_for=dated_url_for)